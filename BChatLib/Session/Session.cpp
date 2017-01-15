#include "Session.h"

const uint32_t Session::BUFFERS_SIZE = 1000000;
//const uint32_t Session::VIDEO_FRAME_CONTAINER_BUFFER_SIZE = 1000000; //
//const uint32_t Session::CHAT_MESSAGE_CONTAINER_BUFFER_SIZE = 1000000;

Session::Session() :
	_frameConverter(QImage::Format_RGB444),
	_multiplexor(BUFFERS_SIZE)
{
}


Session::~Session()
{
	//������� ��� ������������ INetwork
	for(int i = 0; i<_users.size(); i++)
	{
		_users[i].client->Stop();
		delete _users[i].client;
		_users[i].client = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////


//����������� � ����
void Session::JoinChat(uint32_t userId)
{
	uint8_t* keyBuffer = nullptr;

	try
	{
		auto settings = SettingsManagerContainer::Inner()->ReadSettings();
		_port = settings.GetPort();

		//������� ������ cryptoApi
		_cryptoAPI.Init(settings.GetContainer());

		//��������� ���� ������������ ������ ���������� � ������� ����������� ������������,
		//���������� �� ������� ���, �� ����� 1 �������������

		TcpClient client;
		int result = client.Connect((char*)settings.GetIP().c_str(), settings.GetPort());

		if (result != 0)
		{
			Logger::Instance()->WriteException("Can't join chat");
			DialogHelper::ShowDialog("Can't join chat");
			return;
		}

		//�������� ���� Id (��� ������ ���� ���������� Id)
		client.Send((uint8_t*)&_myId, sizeof(_myId));

		//��������� Id ������� ������������ (� ���� ����� ���� ��������� �������������)
		uint32_t userId;
		client.SimpleRecv((uint8_t*)&userId, sizeof(userId));

		//����� �������		
		uint32_t keyBufferSize;

		client.RecvAlloc((uint8_t**)&keyBuffer, &keyBufferSize);
		if (keyBuffer == nullptr)
		{
			Logger::Instance()->WriteException("Can't get session key from server");
			return;
		}

		_cryptoAPI.ImportSessionKey(keyBuffer, keyBufferSize, settings.GetCertificate(), settings.GetInterlocutorCertificate());

		delete[] keyBuffer;

		//��������� ������������ � ������ �������������
		SetupPipeline();
		AddUser(userId, client);
	}
	catch (Exception ex)
	{
		//����� �������� �� ��������� keyBuffer
		DialogHelper::ShowDialog(ex.Message);
		if (keyBuffer != nullptr)
			delete[] keyBuffer;
	}
}

//��������� ��������� � ���
void Session::SendChatMessage(QString message)
{
	_messageConverter.MessageInput(message);
}

//�������� ����
void Session::CreateChat()
{
	auto settings = SettingsManagerContainer::Inner()->ReadSettings();
	_port = settings.GetPort();

	//������� ������ cryptoApi � ���������� ��������� ����
	_cryptoAPI.Init(settings.GetContainer());
	_cryptoAPI.CreateSessionKey();
	
	SetupPipeline();
	this->start();
}


//�������� ����������� � ��������� ������
void Session::run()
{
	TcpListener listener(_port);
	listener.Start();
	Logger::Instance()->Write("Listening started");

	//� �������, ��� ����� ����������� ����� ����������� �������
	while (true)
	{
		TcpClient client = listener.AcceptClient();

		Logger::Instance()->Write("Client connecting...");

		//��������� ������
		try
		{
			//������ Id ������������� ������������
			uint32_t userId;
			client.SimpleRecv((uint8_t*)&userId, sizeof(userId));

			//�������� ���� Id
			client.Send((uint8_t*)&_myId, sizeof(_myId));

			//�������� ������������ �� ����� Id (�� ����� ���� ���, ��� ����������� ������������)
			auto user = UserManagerContainer::Inner()->GetUser(userId);

			//����� �������
			uint8_t* keyBuffer;
			uint32_t keyBufferSize;

			_cryptoAPI.ExportSessionKeyForUser(SettingsManagerContainer::Inner()->ReadSettings().GetCertificate(), user._certName, &keyBuffer, &keyBufferSize);

			client.SendWithLength((uint8_t*)keyBuffer, keyBufferSize);

			delete[] keyBuffer;

			//��������� ������������ � ������ �������������
			AddUser(userId, client);
		}
		catch(Exception ex)
		{
			//������� ���������, �� ��, ����, � ������ ������
		}
	}
}


//////////////////////////////////////// ����� ////////////////////////////////////////////////////


void Session::MyFrameInput(const QVideoFrame & frame)
{
	emit __MyFameInput(frame);
}

void Session::MyMessageInput(const QString string)
{
	emit __MyMessageInput(string);
}

//�������� � ������������ ������ �� ��������
void Session::ConnectionProblem(int errorCode, int clientIndex)
{
	Logger::Instance()->Write(QString("Client #%1 disconnected with error code %2").arg(clientIndex).arg(errorCode));

	INetwork* network = _users[clientIndex].client;
	_users.erase(_users.begin() + clientIndex);

	//������� ������ ����������, �� ������� ��� ������ ���
	if (network != nullptr)
		delete network;

	emit UserDisconnected(clientIndex);
}



///////////////////////////////////////////////////////////////////////////////////////////////////

void Session::AddUser(uint32_t userId, TcpClient tcpClient)
{
	INetwork* client = new NetworkProcessingThread(tcpClient, BUFFERS_SIZE, _userCnt++);

	auto user = UserManagerContainer::Inner()->GetUser(userId);
	SessionUser sessionUser(user, client);
	_users.push_back(sessionUser);


	//���������� - ����
	//connect(&_crypter, SIGNAL(EncryptSignal(uint8_t*, quint32)), client, SLOT(SendSlot(uint8_t*, quint32)));
	connect(&_multiplexor, SIGNAL(OutputData(quint8*, quint32)), client, SLOT(SendSlot(quint8*, quint32)), Qt::DirectConnection);

	//����� - �����������
	//connect(client, SIGNAL(RecvSignal(quint8*, quint32)), &_crypter, SLOT(DecryptSlot(quint8*, quint32)), Qt::DirectConnection);
	connect(client, SIGNAL(RecvSignal(quint8*, quint32)), &_multiplexor, SLOT(InputData(quint8*, quint32)));// , Qt::DirectConnection);



	//����������
	connect(client, SIGNAL(ConnectionProblem(int, int)), this, SLOT(ConnectionProblem(int, int)));

	client->start();

	emit UserConnected((int)userId);

	Logger::Instance()->Write("Client connected");
}


//����������� �������� ��������� ������
void Session::SetupPipeline()
{
	//���������������, ������� ����� � ������
	connect(this, SIGNAL(__MyFameInput(QVideoFrame)), &_frameConverter, SLOT(FrameInput(QVideoFrame)));
	connect(this, SIGNAL(__MyMessageInput(QString)), &_messageConverter, SLOT(MessageInput(QString)));


	//����� ���������� �� ����������� ����� ������
	connect(&_frameConverter, SIGNAL(FrameOutput(QImage&)), this, SIGNAL(MyFrameOutput(QImage&)));

	//��������� - �������������� � ���������
	connect(&_frameConverter, SIGNAL(FrameOutput(QImage&)), &_qimageToContainerConverter, SLOT(FrameInput(QImage&)));

	//��������� ����� - ��������������
	connect(&_qimageToContainerConverter, SIGNAL(DataOutput(const Containers::VideoFrameContainer *)), &_multiplexor, SLOT(InputVideoContainer(const Containers::VideoFrameContainer*)));

	//��������� ��������� - �������������
	connect(&_messageConverter, SIGNAL(DataOutput(const Containers::ChatMessageContainer*)), &_multiplexor, SLOT(InputChatContainer(const Containers::ChatMessageContainer*)));

	//������ �� �������������� - ����������
	//connect(&_multiplexor, SIGNAL(OutputData(uint8_t*, uint32_t)), &_crypter, SLOT(EncryptSlot(uint8_t*, uint32_t)));




	//������������� - �������������
	//connect(&_crypter, SIGNAL(DecryptSignal(uint8_t*, uint32_t)), &_multiplexor, SLOT(InputData(uint8_t*, uint32_t)));

	//������������ - ��������� �����
	connect(&_multiplexor, SIGNAL(OutputFrame(const Containers::VideoFrameContainer *)), &_containerToQImageConverter, SLOT(DataInput(const Containers::VideoFrameContainer *)));// , Qt::DirectConnection);

	//��������� ����� - ����, �����
	connect(&_containerToQImageConverter, SIGNAL(FrameOutput(QImage&)), this, SIGNAL(OtherFrameOutput(QImage&)));// , Qt::DirectConnection);

	//������������� - ���������
	connect(&_multiplexor, SIGNAL(OutputMessage(const Containers::ChatMessageContainer*)), this, SIGNAL(MessageOutput(const Containers::ChatMessageContainer*)));
}

#include "Session.h"

Session::Session() :
	_frameConverter(QImage::Format_RGB444)
{
}


Session::~Session()
{
	//������� ��� ������������ INetwork
	for(int i = 0; i<_users.size(); i++)
	{
		delete _users[i].client;
		_users[i].client = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////


//����������� � ����
void Session::JoinChat(uint32_t userId)
{
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
	//� �������, ��� ����� ����������� ����� ����������� �������
	while (true)
	{

		TcpListener listener(_port);
		listener.Start();
		TcpClient client = listener.AcceptClient();

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

//������� ���� � ���������
void Session::__OtherFrameOutput(QImage & frame)
{
	emit OtherFrameOutput(frame);
}


void Session::MyFrameInput(const QVideoFrame & frame)
{
	emit __MyFameInput(frame);
}

//�������� � ������������ ������ �� ��������
void Session::ConnectionProblem(int errorCode, int clientIndex)
{
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
	INetwork* client = new NetworkProcessingThread(tcpClient, RECV_BUFFER_SIZE, _userCnt++);

	auto user = UserManagerContainer::Inner()->GetUser(userId);
	SessionUser sessionUser(user, client);
	_users.push_back(sessionUser);


	//���������� - ����
	//connect(&_crypter, SIGNAL(EncryptSignal(uint8_t*, uint32_t)), client, SLOT(SendSlot(uint8_t*, uint32_t)));
	connect(&_multiplexor, SIGNAL(OutputData(uint8_t*, uint32_t)), client, SLOT(SendSlot(uint8_t*, uint32_t)));

	//����� - �����������
	//connect(client, SIGNAL(RecvSignal(uint8_t*, uint32_t)), &_crypter, SLOT(DecryptSlot(uint8_t*, uint32_t)));
	connect(client, SIGNAL(RecvSignal(uint8_t*, uint32_t)), &_multiplexor, SLOT(InputData(uint8_t*, uint32_t)), Qt::DirectConnection);

	//����������
	connect(client, SIGNAL(ConnectionProblem(int, int)), this, SLOT(ConnectionProblem(int, int)));

	client->start();

	emit UserConnected((int)userId);
}


//����������� �������� ��������� ������
void Session::SetupPipeline()
{
	//���������������, ������� ����� � ������
	connect(this, SIGNAL(__MyFameInput(QVideoFrame)), &_frameConverter, SLOT(FrameInput(QVideoFrame)));

	//����� ���������� �� ����������� ����� ������
	connect(&_frameConverter, SIGNAL(FrameOutput(QImage&)), this, SIGNAL(MyFrameOutput(QImage&)));

	//��������� - �������������� � ���������
	connect(&_frameConverter, SIGNAL(FrameOutput(QImage&)), &_qimageToContainerConverter, SLOT(FrameInput(QImage&)));

	//��������� ����� - ��������������
	connect(&_qimageToContainerConverter, SIGNAL(DataOutput(const Containers::VideoFrameContainer *)), &_multiplexor, SLOT(InputContainer(const Containers::VideoFrameContainer*)));

	//������ �� �������������� - ����������
	//connect(&_multiplexor, SIGNAL(OutputData(uint8_t*, uint32_t)), &_crypter, SLOT(EncryptSlot(uint8_t*, uint32_t)));

	//������������� - �������������
	//connect(&_crypter, SIGNAL(DecryptSignal(uint8_t*, uint32_t)), &_multiplexor, SLOT(InputData(uint8_t*, uint32_t)));

	//������������ - ��������� �����
	connect(&_multiplexor, SIGNAL(OutputFrame(const Containers::VideoFrameContainer *)), &_containerToQImageConverter, SLOT(DataInput(const Containers::VideoFrameContainer *)));// , Qt::DirectConnection);

																																												 //��������� ����� - ����, �����
	connect(&_containerToQImageConverter, SIGNAL(FrameOutput(QImage&)), this, SLOT(__OtherFrameOutput(QImage&)));// , Qt::DirectConnection);
}

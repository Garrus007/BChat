#include "SimpleSessionManager.h"

SimpleSessionManager::SimpleSessionManager()
{
}

void SimpleSessionManager::CreateChat()
{
	try
	{
		auto settings = SettingsManagerContainer::Inner()->ReadSettings();

		//������� CryptoAPI � ���������� ��������� ����
		_cryptoAPI.Init(settings.GetContainer());
		_cryptoAPI.CreateSessionKey();

		std::thread waitingThread(&SimpleSessionManager::WaitForConnection, this, settings.GetPort());
		waitingThread.detach();

		emit WaitingForConnection();
	}
	catch (Exception ex)
	{
		DialogHelper::ShowDialog(ex.Message.c_str());
	}
}

//����������� � ������� ������������
void SimpleSessionManager::ConnectToUser(uint32_t userId)
{
	uint8_t* keyBuffer = nullptr;

	try
	{
		auto settings = SettingsManagerContainer::Inner()->ReadSettings();

		//������� CryptoAPI
		_cryptoAPI.Init(settings.GetContainer());

		//��������� ���� ������������ ������ ���������� � ������� ����������� ������������,
		//���������� �� ������� ���, �� ����� 1 �������������

		TcpClient client;
		int result = client.Connect((char*)settings.GetIP().c_str(), settings.GetPort());

		if (result != 0)
			DialogHelper::ShowDialog("Can't join chat");

		//�������� ���� Id (��� ������ ���� ���������� Id)
		uint32_t myId = 0;
		client.SimpleSend((char*)&myId, sizeof(myId));

		//����� �������		
		int keyBufferSize;

		client.Recv((char**)&keyBuffer, &keyBufferSize);
		if (keyBuffer == nullptr)
			throw new Exception("Can't get session key from server");

		_cryptoAPI.ImportSessionKey(keyBuffer, keyBufferSize, settings.GetCertificate(), settings.GetInterlocutorCertificate());
	}
	catch (Exception ex)
	{
		DialogHelper::ShowDialog(ex.Message.c_str());
	}
}

//�������� ����������� ������������
void SimpleSessionManager::WaitForConnection(int port)
{
	TcpListener listener(port);
	listener.Start();
	TcpClient client = listener.AcceptClient();

	//���������� �����������
	AcceptConnection(client);

	emit UserConnected();
}

//����� ������� � ������ ���������� � ������������ �������������
void SimpleSessionManager::AcceptConnection(TcpClient  client)
{
	try
	{
		//������ Id ������������� ������������
		uint32_t userId;
		client.SimpleRecv((char*)&userId, sizeof(userId));

		//�������� ������������ �� ����� Id (�� ����� ���� ���, ��� ����������� ������������)
		auto user = UserManagerContainer::Inner()->GetUser(userId);

		//����� �������
		uint8_t* keyBuffer;
		uint32_t keyBufferSize;

		_cryptoAPI.ExportSessionKeyForUser(SettingsManagerContainer::Inner()->ReadSettings().GetCertificate(), user._certName, &keyBuffer, &keyBufferSize);

		client.Send((char*)keyBuffer, keyBufferSize);

		delete[] keyBuffer;
	}
	catch (Exception ex)
	{
		DialogHelper::ShowDialog(ex.Message);
	}
}

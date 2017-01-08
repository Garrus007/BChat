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

void SimpleSessionManager::ConnectToUser(uint32_t userId)
{
}

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
	//������ Id ������������� ������������
	uint32_t userId;
	client.SimpleRecv((char*)&userId, sizeof(userId));
	
	//�������� ������������ �� ����� Id
	//auto user = UserMa
}

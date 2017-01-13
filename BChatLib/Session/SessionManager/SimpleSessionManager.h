#pragma once

#include "Session/Session.h"
#include "ISessionManager.h"

#include "Network/TcpClient.h"
#include "Network/TcpListener.h"

#include "NetworkAdapter/INetwork.h"
#include "NetworkAdapter/NetworkProcessingThread.h"

#include "crypto/cryptoapi.h"
#include "CryptoAdapter/ICrypt.h"
#include "CryptoAdapter/CryptoApiAdapter.h"

#include "util/SettingsManager/SettingsManagerContainer.h"
#include "util/DialogHelper/DialogHelper.h"
#include "UserManager/UserManagerContainer.h"

#include <stdint.h>

#include <qobject>

#include <thread>

/*! 
 * \brief ����������� ���������� ��������� ������

 ������������ ������ ����������� ����-�-������, ������� ��������
 � �������
 */
class SimpleSessionManager : public QObject, public ISessionManager
{
	Q_OBJECT

public:

	SimpleSessionManager();

	/*!
	 * \brief �������� ���� - ������ TcpListener, ��������� ����������� �����
	 ������� ����������� ������������, ����� ������������ ����� ����������� 
	 �������, ������� ������� �������� ��� �������� ����� � ������� ����� ����
	 */
	void CreateChat();

	/*
	 * \breif ����������� - ������ ����������� �� ��������� IP, ����� �������
	 ����������� � �������, ����� ������������ ����� ����������� 
	 �������, ������� ������� �������� ��� �������� ����� � ������� ����� ����
	 */
	void ConnectToUser(uint32_t userId);

	Session& GetSession();

signals:
	void SessionCreated();

private:
	CryptoAPI _cryptoAPI;
	CryptoApiAdapter _cryptoAPIAdapter;
	Session _session;
	uint32_t _myId = 0;

	const uint32_t RECV_BUFFER_SIZE = 1000000;
	

	void WaitForConnection(int port);
	void AcceptConnection(TcpClient & client);

	NetworkProcessingThread* CreateTcpClientAdapter(TcpClient & client);
	//CryptoApiAdapter* CreateCryptoAPIAdapter(CryptoAPI & api);

};
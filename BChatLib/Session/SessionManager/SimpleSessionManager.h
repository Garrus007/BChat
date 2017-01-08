#pragma once

#include "Session/Session.h"
#include "ISessionManager.h"

#include "Network/TcpClient.h"
#include "Network/TcpListener.h"

#include "NetworkAdapter/INetwork.h"
#include "NetworkAdapter/NetworkAdapter.h"*/

#include "crypto/cryptoapi.h"
#include "CryptoAdapter/ICrypt.h"
#include "CryptoAdapter/CryptoApiAdapter.h"

#include "util/SettingsManager/SettingsManagerContainer.h"
#include "util/DialogHelper/DialogHelper.h"
#include "UserManager/UserManagerContainer.h";

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

signals:
	void WaitingForConnection();
	void UserConnected();

private:
	Session _session;
	CryptoAPI _cryptoAPI;
	

	void WaitForConnection(int port);
	void AcceptConnection(TcpClient client);

};
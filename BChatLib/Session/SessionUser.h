#pragma once

#include "Models/User.h"
#include "NetworkAdapter\INetwork.h"
/*!
 * \brief ������ ������ ������������ � ������ - ��� ������������ � TcpClient ��� �����
 */
class SessionUser
{
public:

	//SessionUser();
	SessionUser(User user, INetwork * client);

	User user;
	INetwork* client;
};

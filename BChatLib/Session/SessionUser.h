#pragma once

#include "Models/User.h"
#include "Network/TcpClient.h"

/*!
 * \brief ������ ������ ������������ � ������ - ��� ������������ � TcpClient ��� �����
 */
class SessionUser
{
public:

	SessionUser();
	SessionUser(User user, TcpClient client);

	User user;
	TcpClient client;
};

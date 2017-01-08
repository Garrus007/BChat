#pragma once

#include "Models/User.h"
#include "Network/TcpClient.h"

/*!
 * \brief ������ ������ ������������ � ������ - ��� ������������ � TcpClient ��� �����
 */
class SessionUser
{
public:
	User User;
	TcpClient Client;
};

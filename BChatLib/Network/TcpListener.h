#pragma once
#include "TcpClient.h"
#include "master.h"
#include "common\Exceptions.h"

/*! 
 * \brief ������ TCP
 *
 * ��������� ����������� � ���������� TcpClient.
 * ������������
 * 
 */
class TcpListener
{
public:	
	TcpListener();
	TcpListener(int port);
	~TcpListener();
	
	int Start();				// ������ ������ �������
	TcpClient AcceptClient();	// ������ ������ �����������

private:	
	//char*		_ip;			// Ip-����� �������, ������������ �����������
	int			_port;			// ���� �������, ������������ �����������
	SOCKET		_inSocket;		// �����, �� ������� �������� ����� �����������	
};


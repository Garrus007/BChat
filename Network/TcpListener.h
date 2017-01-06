#pragma once
#include "TcpClient.h"
#include "master.h"

class TcpListener
{
public:	
	TcpListener();
	TcpListener(int port);
	TcpListener(char* ip, int port);
	~TcpListener();
	
	int Start();				// ������ ������ �������
	TcpClient AcceptClient();	// ������ ������ �����������

private:	
	char*		_ip;			// Ip-����� �������, ������������ �����������
	int			_port;			// ���� �������, ������������ �����������
	SOCKET		_inSocket;		// �����, �� ������� �������� ����� �����������	
};


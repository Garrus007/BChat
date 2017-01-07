#pragma once
#include "master.h"

/*!
 * \brief ������������ ������������ ����� ������� �� ���� ��� ������ TCP
 */
class TcpClient
{
public:
	TcpClient(SOCKET socket);	
	TcpClient();
	~TcpClient();	

	SOCKET GetSocket();								// ������ ��� ������
	int Recv(char** message, int* xyuLength);		// �������� ���������
	int Send(char* message, int messageLength);		// ��������� ���������
	int Connect(char* ip, int port);				// ������������

private:
	SOCKET _socket;
};


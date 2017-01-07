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
	int Recv(char** message, int* msgLength);		// �������� ���������
	int Send(char* message, int messageLength);		// ��������� ���������
	int Connect(char* ip, int port);				// ������������
	void Close();									// ������� �����

private:
	SOCKET _socket;
};


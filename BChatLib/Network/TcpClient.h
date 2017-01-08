#pragma once
#include "master.h"
#include <new>          // std::bad_alloc
#include "util\Logger\Logger.h"

/*!
 * \brief ������������ ������������ ����� ������� �� ���� ��� ������ TCP
 */
class TcpClient
{
public:
	TcpClient(SOCKET socket);	
	TcpClient();
	~TcpClient();	

	SOCKET GetSocket();									// ������ ��� ������
	int Recv(char** message, int* msgLength);			// �������� ���������
	int SimpleRecv(char* message, int length);			// ����� �������������� ����� ��� ���������� ����� ���������
	int Send(char* message, int messageLength);			// ��������� ���������
	int SimpleSend(char* message, int messageLength);	// ����� �������������� �������� ��� ���������� �����
	int Connect(char* ip, int port);					// ������������
	void Close();										// ������� �����

private:
	SOCKET _socket;

	//void setOptions(SOCKET sock);

	int bufferSize = 1000000;

	//void flushSocket();
};


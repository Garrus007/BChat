#include "TcpClient.h"

TcpClient::TcpClient(SOCKET socket)
{
	_socket = socket;
	setOptions(socket);
}

TcpClient::~TcpClient()
{	
}

TcpClient::TcpClient()
{
}

void TcpClient::Close()
{
	closesocket(_socket);
}

//������������� ������� ������
void TcpClient::setOptions(SOCKET sock)
{
	DWORD timeout = 1000;

	int result = setsockopt(sock,
		SOL_SOCKET,
		SO_RCVTIMEO,
		(const char *)&timeout,
		sizeof(timeout));

	 result = setsockopt(sock,
		SOL_SOCKET,
		SO_SNDTIMEO,
		(const char *)&timeout,
		sizeof(timeout));
}

void TcpClient::flushSocket()
{
	char buffer[2048];
	int actualLength = 0;

	do
	{
		if (SOCKET_ERROR == (actualLength = recv(_socket, buffer, 2048, 0)))
			return;
		if (actualLength != 2048)
			return;

	} while (true);
}

SOCKET TcpClient::GetSocket()
{
	return _socket;
}

int TcpClient::Recv(char** message, int* msgLength)
{
	int messageLength = 0;
	int actual_len = 0;

	/*if (SOCKET_ERROR == (actual_len = recv(_socket, (char*)&messageLength, sizeof(int), 0)))
	{
		return WSAGetLastError();
	}*/

	messageLength = bufferSize;

	try
	{
		*message = new char[messageLength];
	}
	catch (std::bad_alloc& ba)
	{
		Util::Logger::Instance()->WriteException(QString("Error allocating %1 bytes").arg(messageLength));
		return -1;
	}

	if (SOCKET_ERROR == (actual_len = recv(_socket, *message, messageLength, 0)))
	{
		return WSAGetLastError();				
	}		
	*msgLength = actual_len;
	return 0;
}


int TcpClient::SimpleRecv(char * message, int length)
{
	int actual_len = 0;
	if (SOCKET_ERROR == (actual_len = recv(_socket, message, length, 0)))
		return WSAGetLastError();
	
	return 0;
}

int TcpClient::Send(char* message, int messageLength)
{
	if (messageLength > bufferSize)
	{
		Util::Logger::Instance()->WriteException(QString("Message length is bigger then max length"));
		return;
	}
	
	/*if (SOCKET_ERROR == (send(_socket, (char*)&messageLength, sizeof(int), 0)))
	{
		return WSAGetLastError();
	}*/
	if (SOCKET_ERROR == (send(_socket, message, messageLength, 0)))
	{
		return WSAGetLastError();
	}
	return 0;
}

int TcpClient::SimpleSend(char * message, int messageLength)
{
	if (SOCKET_ERROR == (send(_socket, message, messageLength, 0)))
	{
		return WSAGetLastError();
	}

	return 0;
}

int TcpClient::Connect(char* ip, int port)
{	
	// ������� ���������� ��� �������� ������ 
	sockaddr_in s_address;
	// �������� ��:
	ZeroMemory(&s_address, sizeof(s_address));
	// ��� ������ (TCP/IP)
	s_address.sin_family = AF_INET;
	// ����� �������. �.�. TCP/IP ������������ ������ � �������� ����, �� ��� �������� 
	// ������ ���������� ������� inet_addr.	
	s_address.sin_addr.S_un.S_addr = inet_addr(ip);
	// ����. ���������� ������� htons ��� �������� ������ ����� �� �������� � //TCP/IP �������������.
	s_address.sin_port = htons(port);
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	setOptions(_socket);

	// ������ ��������� ����������:
	if (SOCKET_ERROR == (connect(_socket, (sockaddr *)&s_address, sizeof(s_address))))
	{
		// Error...
		return WSAGetLastError();		
	}
	return 0;
}

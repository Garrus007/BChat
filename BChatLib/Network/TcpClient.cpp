#include "TcpClient.h"

TcpClient::TcpClient(SOCKET socket)
{
	_socket = socket;
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


SOCKET TcpClient::GetSocket()
{
	return _socket;
}

/*!
* \brief �������� ��������� � ��� �����
*
*  ���� ����� ���������� ������������ �����, ������ ��������� ������
*
* \param[in] message - ��������� �� �����, ���� ����� �������� ������
* \param[out] msgLength - ��������� �� ����������, ���� ����� �������� ����� �������� ������
* \param[un] bufferSize - ������ ������
*/
int TcpClient::Recv(uint8_t* message, uint32_t* msgLength, uint32_t bufferSize)
{
	int recvResult = recv(_socket, (char*)message, bufferSize, 0);

	//��������� ������
	if (recvResult == SOCKET_ERROR)
	{
		DWORD error = WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error receiving message, error code: %1").arg(error));
		return error;
	}

	*msgLength = recvResult;

	return 0;
}

/*!
* \brief �������� ��������� � ��� �����
*
*  ���� ����� �������� ����� �����
*
* \param[in] message - ��������� �� �����, ���� ����� �������� ������
* \param[out] msgLength - ��������� �� ����������, ���� ����� �������� ����� �������� ������
*/
int TcpClient::RecvAlloc(uint8_t ** message, uint32_t * msgLength)
{
	//int recvResult;
	uint32_t messageLength;

	if (recv(_socket, (char*)&messageLength, sizeof(messageLength), 0) != sizeof(messageLength))
	{
		DWORD error =  WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error receiving message length, error code: %1").arg(error));
		return error;
	}

	try
	{
		*message = new uint8_t[messageLength];
	}
	catch (std::bad_alloc& ba)
	{
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error allocating %1 bytes").arg(messageLength));
		return -1;
	}

	//��������� ���������
	if (recv(_socket, (char*)*message, messageLength, 0) != messageLength)
	{
		DWORD error = WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error receiving message, error code: %1").arg(error));

		delete[] *message;
		return error;
	}

	*msgLength = messageLength;
}

//���������� ������ �������� ������
int TcpClient::SimpleRecv(uint8_t * message, uint32_t length)
{
	int actual_len ;
	if (SOCKET_ERROR == (actual_len = recv(_socket, (char*)message, length, 0)))
	{
		DWORD error = WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error receiving message, error code: %1").arg(error));
		return error;
	}
	
	return 0;
}

//��������� ���������
int TcpClient::Send(uint8_t* message, uint32_t messageLength)
{
	int sendResut = send(_socket, (char*)message, messageLength, 0);

	if (sendResut == SOCKET_ERROR || sendResut!=messageLength)
	{
		DWORD error = WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error sending message, error code: %1").arg(error));
		return error;
	}

	return 0;
}

//���������� ��������� ������ � ��� ������
int TcpClient::SendWithLength(uint8_t * message, uint32_t messageLength)
{
	if (send(_socket, (char*)&messageLength, sizeof(messageLength), 0) != sizeof(messageLength))
	{
		DWORD error = WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error sending message length, error code: %1").arg(error));
		return error;
	}


	if (send(_socket, (char*)message, messageLength, 0) != messageLength)
	{
		DWORD error = WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error sending message, error code: %1").arg(error));
		return error;
	}
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

	// ������ ��������� ����������:
	if (SOCKET_ERROR == (connect(_socket, (sockaddr *)&s_address, sizeof(s_address))))
	{
		// Error...
		DWORD error = WSAGetLastError();
		Util::Logger::Instance()->WriteException(QString("TCPClient: Error connecting to server, error code: %1").arg(error));
		return error;
	}
	return 0;
}

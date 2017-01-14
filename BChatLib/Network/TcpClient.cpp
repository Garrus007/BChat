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
		throw NetworkException("TCPClient: Error receiving message", WSAGetLastError());

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
		throw NetworkException("TCPClient: Error receiving message length", WSAGetLastError());
		

	try
	{
		*message = new uint8_t[messageLength];
	}
	catch (std::bad_alloc& ba)
	{
		throw new Exception(QString("TCPClient: Error allocating %1 bytes").arg(messageLength));
	}

	//��������� ���������
	if (recv(_socket, (char*)*message, messageLength, 0) != messageLength)
	{
		delete[] * message;
		throw NetworkException("TCPClient: Error receiving message length", WSAGetLastError());
	}
		

	*msgLength = messageLength;
}

//���������� ������ �������� ������
int TcpClient::SimpleRecv(uint8_t * message, uint32_t length)
{
	int actual_len ;
	if (SOCKET_ERROR == (actual_len = recv(_socket, (char*)message, length, 0)))
		throw NetworkException("TCPClient: Error receiving message", WSAGetLastError());
	
	return 0;
}

//��������� ���������
int TcpClient::Send(uint8_t* message, uint32_t messageLength)
{
	int sendResut = send(_socket, (char*)message, messageLength, 0);

	if (sendResut == SOCKET_ERROR || sendResut!=messageLength)
		throw NetworkException("TCPClient: Error sending message", WSAGetLastError());

	return 0;
}

//���������� ��������� ������ � ��� ������
int TcpClient::SendWithLength(uint8_t * message, uint32_t messageLength)
{
	if (send(_socket, (char*)&messageLength, sizeof(messageLength), 0) != sizeof(messageLength))
		throw NetworkException("TCPClient: Error sending message length", WSAGetLastError());


	if (send(_socket, (char*)message, messageLength, 0) != messageLength)
		throw NetworkException("TCPClient: Error sending message", WSAGetLastError());
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
		throw NetworkException("TCPClient: Error connecting to server", WSAGetLastError());

	return 0;
}

#pragma once
#include "master.h"
#include <new>          // std::bad_alloc
#include "util\Logger\Logger.h"
#include <qdebug.h>

/*!
 * \brief ������������ ������������ ����� ������� �� ���� ��� ������ TCP
 */
class TcpClient
{
public:
	TcpClient(SOCKET socket);	
	TcpClient();
	~TcpClient();	

	// ������ ��� ������
	SOCKET GetSocket();

	/*!
	 * \brief �������� ��������� � ��� �����
	 * 
	 *  ���� ����� ���������� ������������ �����, ������ ��������� ������
	 * 
	 * \param[in] message - ��������� �� �����, ���� ����� �������� ������
	 * \param[out] msgLength - ��������� �� ����������, ���� ����� �������� ����� �������� ������
	 * \param[un] bufferSize - ������ ������
	 */
	int Recv(uint8_t* message, uint32_t* msgLength, uint32_t bufferSize);

	/*!
	* \brief �������� ��������� � ��� �����
	*
	*  ���� ����� �������� ����� �����
	*
	* \param[in] message - ��������� �� �����, ���� ����� �������� ������
	* \param[out] msgLength - ��������� �� ����������, ���� ����� �������� ����� �������� ������
	*/
	int RecvAlloc(uint8_t** message, uint32_t* msgLength);

	// �������� ��������� �������� �����
	int SimpleRecv(uint8_t* message, uint32_t length);


	// ��������� ���������
	int Send(uint8_t* message, uint32_t messageLength);

	//���������� ��������� ������ � ��� ������
	int SendWithLength(uint8_t* message, uint32_t messageLength);

	// ������������
	int Connect(char* ip, int port);

	// ������� �����
	void Close();

private:
	SOCKET _socket;
};


#include "NetworkProcessingThread.h"

NetworkProcessingThread::NetworkProcessingThread(TcpClient tcpClient,  uint32_t bufferSize)
{
	_tcpClient = tcpClient;
	_bufferSize = bufferSize;
	_buffer = new uint8_t[bufferSize];
}

/*
��� ������ � ������, � �������� ����
��� ��� �������� � �������

*/

NetworkProcessingThread::~NetworkProcessingThread()
{
	if (_buffer != nullptr)
	{
		delete[] _buffer;
		_buffer = nullptr;
	}
}

void NetworkProcessingThread::SendSlot(uint8_t* data, uint32_t size)
{
	_tcpClient.Send(data, size);
}

void NetworkProcessingThread::run()
{		
	int result;
	uint32_t		actualLength;			// ������ �������� ������

	do
	{
		// ������� ����� ������
		result = _tcpClient.Recv(_buffer, &actualLength, _bufferSize);
		
		if (result == 0)
		{
			// ��������� ������ � ���, ��� ������, �������, �������
			emit RecvSignal(_buffer, actualLength);
		}
		else
		{
			emit ConnectionProblem(result);
			return;
		}

	}
	while (true);
}

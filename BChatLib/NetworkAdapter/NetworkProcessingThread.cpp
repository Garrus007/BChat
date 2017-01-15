#include "NetworkProcessingThread.h"

NetworkProcessingThread::NetworkProcessingThread(TcpClient tcpClient,  uint32_t bufferSize, int clientIndex)
{
	_tcpClient = tcpClient;
	_bufferSize = bufferSize;
	_buffer = new uint8_t[bufferSize];
	_clientIndex = clientIndex;
}

/*
��� ������ � ������, � �������� ����
��� ��� �������� � �������

*/

NetworkProcessingThread::~NetworkProcessingThread()
{
	_tcpClient.Close();

	if (_buffer != nullptr)
	{
		delete[] _buffer;
		_buffer = nullptr;
	}

	while (this->isRunning());
}

void NetworkProcessingThread::Stop()
{
	
}


void NetworkProcessingThread::SendSlot(quint8* data, quint32 size)
{
	try
	{
		_tcpClient.Send(data, size);
	}
	catch (NetworkException ex)
	{
		emit ConnectionProblem(ex.ErrorCode, _clientIndex);
	}
}


void NetworkProcessingThread::run()
{		
	int result;
	uint32_t		actualLength;			// ������ �������� ������

	do
	{
		try
		{
			// ������� ����� ������
			_tcpClient.Recv(_buffer, &actualLength, _bufferSize);

			// ��������� ������ � ���, ��� ������, �������, �������
			emit RecvSignal((quint8*)_buffer, (quint32)actualLength);
		}
		catch (NetworkException ex)
		{
			//��� ����� ���������� ��������
			//����� ����������, ������ ������������, � ����� �����������.
			//�� ����� ���-����� ����������� ���� ����� � emit �������� ������
			if(ex.ErrorCode!=-1)
				emit ConnectionProblem(ex.ErrorCode, _clientIndex);

			return;
		}
	}
	while (true);
}

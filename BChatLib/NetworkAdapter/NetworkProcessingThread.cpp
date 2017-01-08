#include "NetworkProcessingThread.h"

NetworkProcessingThread::NetworkProcessingThread(TcpClient tcpClient)
{
	_tcpClient = tcpClient;
}

NetworkProcessingThread::~NetworkProcessingThread()
{
}

void NetworkProcessingThread::SendSlot(uint8_t* data, uint32_t size)
{
	_tcpClient.Send((char*)data, size);

	//�������, ��� ������ ������ �� �����
	delete[] data;
	data = nullptr;
}

void NetworkProcessingThread::run()
{		
	int result;
	do
	{
		char*	receivedData;	// ������� ����� ������
		int		size;			// ������ �������� ������

		// ������� ����� ������
		result = _tcpClient.Recv(&receivedData, &size);
		
		if (result == 0)
		{
			// ��������� ������ � ���, ��� ������, �������, �������
			emit RecvSignal((uint8_t*)receivedData, size);
		}
		else
		{
			emit ConnectionProblem(result);
		}

		// TODO: �� ������ �� ������� ����
		//// �������� ������
		//ZeroMemory(receivedData, size);

	} while (true);
}

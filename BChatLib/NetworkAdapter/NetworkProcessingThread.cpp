#include "NetworkProcessingThread.h"

NetworkProcessingThread::NetworkProcessingThread(TcpClient & tcpClient)
{
	_tcpClient = tcpClient;
}

NetworkProcessingThread::~NetworkProcessingThread()
{
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
			return;
		}

		// TODO: �� ������ �� ������� ����
		//// �������� ������
		//ZeroMemory(receivedData, size);

	} while (result == 0);
}

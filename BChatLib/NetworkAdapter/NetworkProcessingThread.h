#pragma once

#include <QObject>
#include <qthread.h>
#include "Network\TcpClient.h"
#include "INetwork.h"

class NetworkProcessingThread : public INetwork
{
	Q_OBJECT

public:
	NetworkProcessingThread(TcpClient);
	~NetworkProcessingThread();

	void run();

public slots :
	virtual void SendSlot(uint8_t*, uint32_t) override;


private:
	TcpClient _tcpClient;
};

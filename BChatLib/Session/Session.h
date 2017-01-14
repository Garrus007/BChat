#pragma once	

#include <qobject.h>
#include <qimage>
#include <qvideoframe.h>
#include <vector>
#include <string>
#include "SessionUser.h"


#include "webcam/CameraFrameGrabber/CameraFrameGrabber.h"
#include "webcam/FrameConverter/FrameConverter.h"
#include "webcam/QImageConverter/QImageToContainerConverter.h"
#include "webcam/QImageConverter/ContainerToQImageConverter.h"

#include "Containers/SimpleContainerMultiplexor.h"
#include "UserManager/UserManagerContainer.h"

#include "NetworkAdapter/INetwork.h"
#include "Network/TcpListener.h"
#include "NetworkAdapter/NetworkProcessingThread.h"

#include "CryptoAdapter/ICrypt.h"
#include "crypto/cryptoapi.h"

//class SimpleSessionManager;

/*!
 * \brief ������ ������. ������������� ��� ������� �� ������ �������
          � ���������� �������������� � ������
 */

//class SimpleSessionManager;

using namespace Crypto;

class Session: public QThread
{
	Q_OBJECT

public:

	Session();
	~Session();

	/*!
	* \brief �������� ���� - ������ TcpListener, ��������� ����������� �����
	������� ����������� ������������, ����� ������������ ����� �����������
	�������, ������� ������� �������� ��� �������� ����� � ������� ����� ����
	*/
	void CreateChat();

	/*
	* \breif ����������� - ������ ����������� �� ��������� IP, ����� �������
	����������� � �������, ����� ������������ ����� �����������
	�������, ������� ������� �������� ��� �������� ����� � ������� ����� ����
	*/
	void JoinChat(uint32_t userId);

	//�������� ����������� � ��������� ������
	void run();

public slots:

	//������� ���� � ���������
	void MyFrameInput(const QVideoFrame&);
	void __OtherFrameOutput(QImage&);

	//�������� � ������������ � ������ �� ��������
	void ConnectionProblem(int, int);

signals:

	//����� ���������� ���� �� ������ ��� ����������� �������������
	void __MyFameInput(const QVideoFrame&);

	void UserConnected(int);
	void UserDisconnected(int);

	//����� �� ������� ������������
	void OtherFrameOutput(QImage&);

	//����� � ���� ������
	void MyFrameOutput(QImage&);



private:
	//������������ ������������
	std::vector<SessionUser> _users;
	int _userCnt = 0;
	int _myId = 0;

	//����� ���������
	Webcam::FrameConverter _frameConverter;
	Webcam::QImageToContainerConverter _qimageToContainerConverter;
	Webcam::ContainerToQImageConverter _containerToQImageConverter;
	Containers::SimpleContainerMultiplexor _multiplexor;

	//������-���
	CryptoAPI _cryptoAPI;
	ICrypt* _crypter;

	//����
	int _port;
	uint32_t RECV_BUFFER_SIZE = 1000000;

	//��������� ������������
	void AddUser(uint32_t userId, TcpClient  client);




	//����������� ��������
	void SetupPipeline();
};
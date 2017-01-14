#pragma once	

#include <qobject.h>
#include <qimage>
#include <qvideoframe.h>
#include <map>

#include "SessionUser.h"


#include "webcam/CameraFrameGrabber/CameraFrameGrabber.h"
#include "webcam/FrameConverter/FrameConverter.h"
#include "webcam/QImageConverter/QImageToContainerConverter.h"
#include "webcam/QImageConverter/ContainerToQImageConverter.h"

#include "Containers/SimpleContainerMultiplexor.h"
#include "UserManager/UserManagerContainer.h"

#include "NetworkAdapter/INetwork.h"
#include "Network/TcpListener.h"

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

	void UserConnected(uint32_t userId, INetwork * client);

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

	void MyFrameInput(const QVideoFrame&);
	void __OtherFrameOutput(QImage&);

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
	std::map<uint32_t, SessionUser> _users;
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

	void SetupPipeline();

};
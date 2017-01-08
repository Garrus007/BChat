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

#include "NetworkAdapter\INetwork.h"
#include "CryptoAdapter\ICrypt.h"

//class SimpleSessionManager;

/*!
 * \brief ������ ������. ������������� ��� ������� �� ������ �������
          � ���������� �������������� � ������
 */

//class SimpleSessionManager;

class Session: public QObject
{
	Q_OBJECT

public:

	Session(ICrypt& crypter);
	void UserConnected(uint32_t userId, INetwork * client);

public slots:

	void MyFrameInput(const QVideoFrame&);

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
	std::map<uint32_t, SessionUser> _users;

	Webcam::FrameConverter _frameConverter;
	Webcam::QImageToContainerConverter _qimageToContainerConverter;
	Webcam::ContainerToQImageConverter _containerToQImageConverter;

	Containers::SimpleContainerMultiplexor _multiplexor;

	ICrypt& _crypter;
};
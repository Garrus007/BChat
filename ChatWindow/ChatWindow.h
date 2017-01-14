#pragma once

#include <QMainWindow>
#include "ui_ChatWindow.h"

#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QCameraInfo>
#include <QMediaMetaData>

#include "Session/Session.h"
#include "webcam/CameraFrameGrabber/CameraFrameGrabber.h"
#include "Containers/VideoFrameContainer.h"

class ChatWindow : public QMainWindow
{
	Q_OBJECT

public:
	//ChatWindow(QWidget *parent = Q_NULLPTR);
	ChatWindow(Session* session);
	~ChatWindow();

	public slots:

	//������� ��������� ��������� ������
	void UpdateCameraState(const QCamera::State state);

	//������� ������� �� ������ ���\���� ������
	void ButtonCameraToggle_clicked();

	//������� ������ ������ � ������ ����
	void CameraSelected(QAction *action);

	void UserConnected(int);
	void UserDisconnected(int);

private:
	Ui::ChatWindow ui;
	Session* _session;

	void UpdateCameras();
	void SetCamera(const QCameraInfo & info);

	QCamera *camera;
	CameraFrameGrabber _cameraFrameGrabber;
	bool _isCameraActive;
};

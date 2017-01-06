#pragma once

#include <QObject>
#include <qdatetime.h>
class Logger : public QObject
{
	Q_OBJECT

public:
		
	void Write(QString);			// ������ ������ � ���
	void WriteDebug(QString);		// ������ ������ � ������ � ���
	bool SetWriteDebugFlag(bool);	// ���������� ���� ������ ������ � ���

	static Logger* Instance()
	{
		if (!_instance)
		{
			// ������
			_instance = new Logger();

			// ���� ������ ������
			_writeDebugFlag = false;

			// �������� ����� ����� �����
			QString currentTime = QDateTime::currentDateTime()
				.toString("dd.MM.yyyy hh:mm:ss")
				.replace(":", "_");

			_filename = currentTime.append(" Logs.log");
		}
		return _instance;
	}

private:
	Logger(QObject *parent);
	Logger();
	Logger(const Logger&);
	Logger& operator=(Logger&);
	~Logger();
	
	static Logger*		_instance;			// ������
	static bool			_writeDebugFlag;	// ���� ������ ������ � ���
	static QString		_filename;			// ��� ����� �����
};

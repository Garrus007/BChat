#pragma once
#include <qdatetime.h>
#include <qstring.h>

/*!
 * \brief ��������-����� ��� ������������� ������������
 */
namespace Util
{
	class Logger
	{
	public:

		void Write(QString);			// ������ ������ � ���
		void WriteDebug(QString);		// ������ ������ � ������ � ���
		bool SetWriteDebugFlag(bool);	// ���������� ���� ������ ������ � ���
		void WriteException(QString);	// ������ ������ � ��� �� ����������

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
		Logger();
		~Logger();
		Logger(const Logger&) = delete;
		Logger& operator=(Logger&) = delete;

		static Logger*		_instance;			// ������
		static bool			_writeDebugFlag;	// ���� ������ ������ � ���
		static QString		_filename;			// ��� ����� �����
	};
}
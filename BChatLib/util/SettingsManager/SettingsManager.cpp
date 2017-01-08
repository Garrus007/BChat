#include "SettingsManager.h"
#include <fstream>
#include "common\Exceptions.h"

SettingsManager::SettingsManager()
{
}


SettingsManager::~SettingsManager()
{
}

Settings SettingsManager::ReadSettings()
{
	// ��������� �� �����
	fstream fs;
	fs.open(_filename, std::fstream::in | fstream::binary);
	
	if (fs.is_open())
	{
		// ������� ����� �����������
		int certLength;
		fs.read((char*)&certLength, sizeof(int));
		// ������� ����������
		char* cert = new char[certLength];
		fs.read(cert, certLength);

		// ������� ����� ����������� �����������
		int interlocutorCertLength;
		fs.read((char*)&interlocutorCertLength, sizeof(int));
		// ������� ���������� �����������
		char* interlocutorCert = new char[interlocutorCertLength];
		fs.read(interlocutorCert, interlocutorCertLength);


		// ������� ����� ����������
		int containerLength;
		fs.read((char*)&containerLength, sizeof(int));
		// ������� ���������
		char* container = new char[containerLength];
		fs.read(container, containerLength);

		// ������� ����� ip
		int ipLength;
		fs.read((char*)&ipLength, sizeof(int));
		char* ip = new char[ipLength];
		fs.read(ip, ipLength);

		// ������� ����
		int port;
		fs.read((char*)&port, sizeof(int));

		// ������� ���������
		Settings settings = Settings(container, cert, interlocutorCert, ip, port);

		// ���������� ������
		delete[] cert;
		delete[] container;
		delete[] ip;
		delete[] interlocutorCert;

		return settings;
	}
	else
	{
		throw Exception("Can't open settings file");
	}
}

void SettingsManager::SaveSettings(Settings settings)
{
	// ��������� � ����
	fstream fs;
	fs.open(_filename, fstream::out | fstream::trunc | fstream::binary);	
	if (fs.is_open())
	{		
		// ��������� ����������
		int tmpLength = settings.GetCertificate().length() + 1;
		fs.write((char*)&tmpLength, sizeof(int));
		fs.write(settings.GetCertificate().c_str(), settings.GetCertificate().length() + 1);

		// ��������� ���������� �����������
		tmpLength = settings.GetInterlocutorCertificate().length() + 1;
		fs.write((char*)&tmpLength, sizeof(int));
		fs.write(settings.GetInterlocutorCertificate().c_str(), settings.GetInterlocutorCertificate().length() + 1);

		// ��������� ���������
		tmpLength = settings.GetContainer().length() + 1;
		fs.write((char*)&tmpLength, sizeof(int));
		fs.write(settings.GetContainer().c_str(), settings.GetContainer().length() + 1);

		// ��������� ip
		tmpLength = settings.GetIP().length() + 1;
		fs.write((char*)&tmpLength, sizeof(int));
		fs.write(settings.GetIP().c_str(), settings.GetIP().length() + 1);
				
		// ��������� ����
		int port = settings.GetPort();
		fs.write((char*)&port, sizeof(int));
		
		fs.close();
	}
	else
	{
		throw Exception("Can't open settings file");
	}
}

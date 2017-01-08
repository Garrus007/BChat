#pragma once
#include <iostream>
using namespace std;

class Settings
{
public:
	Settings(string container, 
		string certificate, 
		string ip, 
		int port);

	~Settings();

	string	GetContainer();
	string	GetCertificate();
	string	GetIP();
	int		GetPort();

private:
	string			_container;		// ��������� (��� ���������)
	string			_certificate;	// ����������
	string			_ip;			// IP �������
	int				_port;			// ���� �������
};


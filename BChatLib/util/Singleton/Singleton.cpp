#include "Singleton.h"

//���������� ������ ���������
Singleton * Singleton::Instance()
{
	if (!_instance)
		_instance = new Singleton();

	return _instance;
}

Singleton::Singleton()
{
}

Singleton::~Singleton()
{
}
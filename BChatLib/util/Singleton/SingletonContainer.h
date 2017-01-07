#ifndef __SINGLETONCONTAINER_H__
#define __SINGLETONCONTAINER_H__

#include "Singleton.h"

/*!
 * \brief ��������-��������� ����� ������� ������������ ��-��������
          ����� � ����������� ��� �� Instance(), ������ ������������
		  instance

	�������������:

	��������, ���������� ��������� �������, ������� ��������� ���������
	��� ��������� �����-�� ����� � ������ ���� ����������.
	��� ���� �� ������ ����� ������ ��� ������ ���� �� ������. 

 */
template<class T>
class SingletonContainer : private Singleton
{
public:

	T&& Inner()
	{
		return SingletonContainer::Instance()->_innerObject;
	}

protected:

	T _innerObject;
};

#endif

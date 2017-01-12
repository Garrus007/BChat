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

	��� ����� ���� ����������� �����-��������� (��������� �����, ������������� ��
	FactoryMethod, ������� ������� ��������� ������� ������.

	����� ����������� ���� �������� �� SingletonContainer<MyClass, MyClassCreator>

	����� �������� ��������� ����������:
	 - ��� �������� ���������� ���������� ������
	 - ��� �������� ������ � ���������

 */

namespace Util
{

	template<class T, class CREATOR>
	class SingletonContainer
	{
	public:

		static T* Inner()
		{
			return SingletonContainer<T, CREATOR>::Instance()->_innerObject;
		}

	protected:

		SingletonContainer()
		{
			_innerObject = _creator.Create();
		}
		~SingletonContainer()
		{
			if (_innerObject != NULL)
				delete[] _innerObject;
		}

		SingletonContainer(const SingletonContainer<T, CREATOR>& other) = delete;
		SingletonContainer<T, CREATOR>& operator=(SingletonContainer<T, CREATOR>& other) = delete;


		static SingletonContainer<T, CREATOR>* Instance()
		{
			if (!_instance)
				_instance = new SingletonContainer<T, CREATOR>();

			return _instance;
		}

		static SingletonContainer<T, CREATOR>* _instance;

		CREATOR _creator;
		T* _innerObject;
	};

	template <class T, class CREATOR>
	SingletonContainer<T, CREATOR>* SingletonContainer<T, CREATOR>::_instance;

}

#endif

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

/*!
 * \brief ��������� ������� ����� ���������, ����� ������ ������ ���������
 */
class Singleton
{
public:

	/*!
	 * \brief ���������� ������ ���������
	 */
	static Singleton* Instance();

protected:
	Singleton();
	~Singleton();
	Singleton(const Singleton& other) = delete;
	Singleton& operator=(Singleton& other) = delete;

	static Singleton* _instance;
};

#endif
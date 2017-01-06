#ifndef __SMARTPOINTERCREATOR_H__
#define __SMARTPOINTERCREATOR_H__


#include <memory>

/*!
 * \brief ������������ ����� ��������� � �������� deleter ��� ���������� 
   ���������
 */
template <class T>
class SmartPointerCreator
{
public:
	static std::shared_ptr<T> CreatePointer(T* t)
	{
		return std::shared_ptr<T>(t, deleter);
	}

private:

	std::shared_ptr<T> _ptr;
	virtual void deleter(T* t) = 0;
};

#endif
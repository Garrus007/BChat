#ifndef __SMARTPOINTERDELETERS_H__
#define __SMARTPOINTERDELETERS_H__

/*!
 * \brief ������� ����� deleter ��� ������������ �������� ��� �������������
 * ����� ����������
 *
 * ���� ������ ��������� ����������� ���-�� �� �������, ���������� 
 * deletor � ��������� � ����������� shared_ptr
 *
 * ��� unique_ptr ���-�� ��-�������
 */
template <class T>
class SmartPointerDeleter
{
public:
	virtual void operator()(T* pointer) = 0;
};

#endif

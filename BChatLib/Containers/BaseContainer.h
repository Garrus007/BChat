#ifndef __BASECONTAINER_H__
#define __BASECONTAINER_H__

#include <stdint.h>

namespace Containers
{

	/*!
	 * \brief ������� ����� ����������
	 *
	 * ��������� ������������� � ���� ��������� ��������� ������,
	 * ��������, ����������, ���������� ���
	 *
	 * ������������� �������
	 */
	class BaseContainer
	{
	public:

		/*!
		 * \brief ���������� ����������� ������ ��� ������������
		 */
		virtual uint32_t GetSize() = 0;

		/*!
		 * \brief ����������� ���� � ����� �� ���������� ���������
		 *
		 * ��������: � ������ ������ ���� ��� ������� GetSize() ����
		 */
		virtual void Serialize(uint8_t* buffer);

		/*!
		 * \brief ������������� ����� � ���� (��� ������������ ������)
		 */
		virtual void Deserialize(uint8_t* buffer);
	};

}

#endif

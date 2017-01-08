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
		virtual uint32_t GetSize() const = 0;

		/*!
		 * \brief ����������� ���� � ����� �� ���������� ���������
		 *
		 * ��������: � ������ ������ ���� ��� ������� GetSize() ����
		 */
		virtual void Serialize(uint8_t* buffer) const = 0;

		/*!
		 * \brief ������������� ����� � ���� (��� ������������ ������)
		 */
		virtual void Deserialize(const uint8_t* buffer) = 0;
	};

}

#endif

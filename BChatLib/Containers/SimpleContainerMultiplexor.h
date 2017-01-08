#pragma once

#include <qobject.h>
#include "BaseContainer.h"
#include "VideoFrameContainer.h"

namespace Containers
{
	/*!
	 * \brief ��������� �������� ��������� �� ��������� ���������� ������ �����
	          � ������������ �� ��������� ����������� ���������
		
		��� ������ ������������ ������ ���� �����������, ��������� �������������,
		� ������ � ������... �� ��� ����� ����������� �����

	 */
	class SimpleContainerMultiplexor : public QObject
	{
		Q_OBJECT

	public:

	public slots:

		/*
		 * \brief ��������� ������ �� ������-�� ���������
		 *
		 * (����, ���� ������ �� ���������)
		 * ������-�� ���� �������� ��������� BaseContainer, �� �����-������� Qt
		 * �� �����������
		 */
		void InputContainer(const Containers::VideoFrameContainer&);
		
		/*
		 * \brief ��������� ������ � �������� ����
		 */
		void InputData(uint8_t*, uint32_t);

	signals:

		/*!
		 * \brief �������� ��������������� ������� ���� ������
		 */
		void OutputData(uint8_t*, uint32_t);

		/*!
		 * \brief �������� ����������� �����
		 */
		void OutputFrame(const Containers::VideoFrameContainer&);
	
	private:
		
		static const uint32_t _headerSize = 6; //sizeof �� �������� ������-��
		const uint8_t _header[_headerSize] = { 0x74, 0x75, 0x15, 0x37, 0xAA, 0xBB };

		//�� ����� ���� ��� ������� ������������ ������
		VideoFrameContainer container;

	};
}

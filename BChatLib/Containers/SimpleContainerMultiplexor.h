#pragma once

#include <qobject.h>
#include "BaseContainer.h"
#include "VideoFrameContainer.h"
#include "ChatMessageContainer.h"

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
		SimpleContainerMultiplexor(uint32_t bufferSize);
		~SimpleContainerMultiplexor();

	public slots:

		/*
		 * \brief ��������� ������ �� ������-�� ���������
		 *
		 * (����, ���� ������ �� ���������)
		 * ������-�� ���� �������� ��������� BaseContainer, �� �����-������� Qt
		 * �� �����������
		 */
		void InputVideoContainer(const Containers::VideoFrameContainer*);
		void InputChatContainer(const Containers::ChatMessageContainer*);
	

		/*
		 * \brief ��������� ������ � �������� ����
		 */
		void InputData(quint8*, quint32);

	signals:

		/*!
		 * \brief �������� ��������������� ������� ���� ������
		 */
		void OutputData(quint8*, quint32);

		/*!
		 * \brief �������� ����������� �����
		 */
		void OutputFrame(const Containers::VideoFrameContainer*);

		/*!
		 * \brief �������� ����������� ��������� � ���
		 */
		void OutputMessage(const Containers::ChatMessageContainer*);
	
	private:
		
		static const uint32_t _headerSize = 6; //sizeof �� �������� ������-��
		const uint8_t _header[_headerSize] = { 0x74, 0x75, 0x15, 0x37, 0xAA, 0xBB };

		//�� ����� ���� ��� ������� ������������ ������
		VideoFrameContainer _videoFrameContainer;
		ChatMessageContainer _chatMessageContainer;

		uint8_t* _sendBuffer;
		uint32_t _bufferSize;

		void InputContainer(const Containers::BaseContainer*);

	};
}

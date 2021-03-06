#include "SimpleContainerMultiplexor.h"

using namespace Containers;

//��������� ������ �� ������-�� ���������
void SimpleContainerMultiplexor::InputContainer(const Containers::VideoFrameContainer* container)
{
	try
	{
		uint32_t size = _headerSize; // +sizeof(userId)
		size += container->GetSize();

		uint8_t * buffer = new uint8_t[size];
		uint8_t * buffer0 = buffer;

		memcpy(buffer, _header, _headerSize);
		buffer += _headerSize;

		//memcpy(buffer, &userId, sizeof(userId));
		//buffer += sizeof(userId);

		container->Serialize(buffer);

		emit OutputData(buffer0, size);
	}
	catch (Exception ex)
	{
		//TODO:  �������� ���������� ����
	}
}

//��������� ������ � �������� ����
void SimpleContainerMultiplexor::InputData(uint8_t * buffer, uint32_t size)
{
	try
	{
		//uint32_t userId;
		uint8_t* buffer0 = buffer;

		if (buffer == nullptr)
		{
			//throw new Exception("Input data is NULL");
			Logger::Instance()->WriteException("Input data is NULL");
			return;
		}

		//���������� ���������
		if (memcmp(buffer, _header, _headerSize) != 0)
		{
			//throw new Exception("Input data is invalid, maybe Session Key is invalid");
			Logger::Instance()->WriteException("Input data is invalid, maybe Session Key is invalid");
			delete[] buffer0;
			return;
		}

		buffer += _headerSize;

		//������ Id ������������
		//memcpy(&userId, buffer, sizeof(userId));
		//buffer += sizeof(userId);

		//TODO: ���������� ��� ����������...

		//������ ����
		container.Deserialize(buffer);
		emit OutputFrame(&container);

		//��� ������������, ��� ��� - ��������� ����� � �������, � 
		//���������� �������� ������ ����, � �� ������ ���������
		delete[] buffer0;
	}
	catch (Exception ex)
	{
		//TODO: �������� ���������� ����
	}
}

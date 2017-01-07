#include "QImageToBytesConverter.h"

void QImageToBytesConverter::FrameInput(QImage& image)
{
	uint32_t size = image.byteCount();
	uint8_t* data = (uint8_t*)image.bits();
	
	//���� ���������� bits(), ������� ���� ��������, �� ��
	//� ���� ��� ������������� �� ������ ���� ������, ����
	//QFrame � QImage ����������� � ��� �������������
	//�� ���� ���� ����������

	//��� ��� ��� �� �����: �� �������� ��������� �� ���������� ������ ���� �� QIMAGE,
	//� ����� �� ������ ������, ��� � �������� ����� ����� ���� ������ ������

	int width = image.width();
	int height = image.height();

	emit DataOutput(data, size);

}
#ifndef __QIMAGETOBYTESCONVERTER_H__
#define __QIMAGETOBYTESCONVERTER_H__

#include <qimage.h>
#include <qobject.h>
#include "common\Exceptions.h"

/*!
 * \brief ������������ QImage � ������ ����
 *
 * ������ ���� ���������� ��� ������ ������ ������� �����������, �
 * ����� ������ ����������� QImage ������ ��� ���������� � ����
 */
class QImageToBytesConverter : public QObject
{
	Q_OBJECT

public:
	QImageToBytesConverter();
	~QImageToBytesConverter();

public slots:
	void FrameInput(QImage&);

signals:
	void DataOutput(uint8_t* data, uint32_t size);

private:
	uint32_t _bufferSize;
	uint8_t* _buffer;
};

#endif

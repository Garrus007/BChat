#ifndef __CRYPTOEXCEPTIONS_H__
#define __CRYPTOEXCEPTIONS_H__

//#include <qstring.h>
#include <string>
#include "common\Exceptions.h"

namespace Crypto
{

	/*!
	 * \brief ������� ��� ���������� ����������������� �������
	 */
	class CryptoException :public Exception
	{
	public:

		CryptoException() :Exception() {}
		CryptoException(ExceptionString message) :Exception(message) {}
	};
}

#endif
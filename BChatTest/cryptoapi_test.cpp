#include "CppUnitTest.h"
#include "TestTestClass.h"

#include "crypto\cryptoapi.h"
#include "webcam\FrameConverter\FrameConverter.h"

using namespace Crypto;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace MyUnitTests
{
	const std::string ContainerName = "TestCertContainer";
	const std::string Cert1Name = "Markov Alexey";
	const std::string Cert2Name = "Garrus Vakarian";

	TEST_CLASS(MyTestClass)
	{
	public:

		//��������� ������� - �����
		TEST_METHOD(Create_CryptoAPI_correct)
		{
			Crypto::CryptoAPI api();
			//�� ���������� - ������
		}

		//�������� ������ - ��������� ����������
		TEST_METHOD(Create_CryptoAPI_Invalid_storage_name)
		{
			try
			{
				Crypto::CryptoAPI api();
				Assert::Fail(); //������ ������ ����������, ���� �� ������ - ������
			}
			catch (CryptoException ex)
			{
				//������ ���������� - ������
			}
		}
		
		//�������� ����������� �����
		TEST_METHOD(Create_sessionKey)
		{
			Crypto::CryptoAPI api();
			api.CreateSessionKey();
			//�� �����
		}

		//������� ����������� ����� - �����
		TEST_METHOD(Export_sessionkey_Correct)
		{
			Crypto::CryptoAPI api();
			api.CreateSessionKey();

			uint8_t* buffer;
			uint32_t size;
			api.ExportSessionKeyForUser(Cert1Name, Cert2Name, &buffer, &size);
			//�� �����
		}

		//������� ����������� ����� - ���������� ������������ �� ������
		TEST_METHOD(Export_sessionkey_Certificate_not_found)
		{
			Crypto::CryptoAPI api();
			api.CreateSessionKey();

			try
			{
				uint8_t* buffer;
				uint32_t size;
				api.ExportSessionKeyForUser(Cert1Name, "Bad name", &buffer, &size);
				
				Assert::Fail(); //������ ������ ����������
			}
			catch (CryptoException ex)
			{
				//������
			}
		}

		//���������� - �����������
		TEST_METHOD(Export_import_sessionkey)
		{
			Crypto::CryptoAPI api();
			api.CreateSessionKey();

			uint8_t* buffer;
			uint32_t size;

			api.ExportSessionKeyForUser(Cert1Name, Cert2Name, &buffer, &size);
			api.ImportSessionKey(buffer, size, Cert2Name, Cert1Name);

			//�� �����
		}

		TEST_METHOD(Encrypt_decrypt)
		{
			uint32_t dataSize = 1000;
			uint8_t* data = new uint8_t[dataSize];
			uint8_t* data2 = new uint8_t[dataSize];

			for (int i = 0; i < dataSize; i++)
				data[i] = '0' + i % 10;

			memcpy(data2, data, dataSize);

			Crypto::CryptoAPI api();
			api.CreateSessionKey();

			api.Encrypt(data, dataSize);
			api.Decrypt(data, dataSize);

			//������ �������� � ����������
			for (int i = 0; i < dataSize; i++)
			{
				if (data[i] != data2[i])
					Assert::Fail();
			}
		}

		TEST_METHOD(Export_certificate_Correct)
		{
			uint8_t* buffer;
			uint32_t size;
			Crypto::CryptoAPI api();


			api.ExportMyCertificate(Cert1Name, &buffer, &size);
		}

		TEST_METHOD(Export_certificate_Certificate_not_found)
		{
			uint8_t* buffer;
			uint32_t size;
			Crypto::CryptoAPI api();

			try
			{
				api.ExportMyCertificate("Bad name", &buffer, &size);
			}
			catch (CryptoException ex)
			{

			}
		}

	};
}
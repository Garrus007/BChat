#include "cryptoapi.h"

using namespace Crypto;

Crypto::CryptoAPI::CryptoAPI():_sessionKeyMutex()
{
}

CryptoAPI::CryptoAPI(std::string containerName):CryptoAPI()
{
	Init(containerName);
}


CryptoAPI::~CryptoAPI()
{

}

void Crypto::CryptoAPI::Init(std::string containerName)
{

	//�������� ���������
	if (!CryptAcquireContextA(
		&*_hCryptProv,
		containerName.c_str(),	//��� ����������
		NULL,					//�� ��������� ����������?
		PROV_GOST_2012_256,
		CRYPT_VERIFYCONTEXT
	))
	{
		/*
		�� ������� �������� ��������� � �������� ������ �����������
		�������������� ������� ��������� �����, �� ������ ��� �� ��������
		��� �� ��� ���, ���� � ���� �� ����� �������� ���� ��� ���������� (???).

		������� ���� ������� ���������� � ���������� ��� � ��������
		https://www.cryptopro.ru/certsrv/certrqma.asp (�������� �� ������-���)

		��� ����� ������� "������������ ����� ������" ��� "����� ����� ������"
		���� ����� - �� ��������� ��������� � ������������ ����� ����.

		�� ��������� ���� ���� ����? ��� ���� �� ������ ����������?

		*/

		//throw CryptoException(QString("Cryptographic context with container \"%1\" couldn't be accured").arg(containerName));
		throw CryptoException("Cryptographic context with container \"%1\" couldn't be accured");
	}

	//�������� ���������. ��� ������ ���� ���������� ��� ���������� � �����������
	//������ - BlockSizeBits/8. ��� ����-8147 ������ ����� �������� 64 ���
	memset(_keyParams, 0, 64 / 8);
}

///////////////////////////////////////////////////////////////////////////////////////////////////


//���������� ���������� ����
bool CryptoAPI::CreateSessionKey()
{	

	_sessionKeyMutex.lock();
	bool result =  CryptGenKey(*_hCryptProv, CALG_G28147, CRYPT_EXPORTABLE, &*_hSessionKey);
	_sessionKeyMutex.unlock();

	return result;
}

//������������ ���������� ���� � ������� �������� ������
//���������������� ������������
void CryptoAPI::ExportSessionKeyForUser(std::string myCertSubjectString, std::string responderCertSubjectString, uint8_t** publicKeyBlob, uint32_t* blobSize)
{
	HCRYPTPROV_SimpleDeleter hProvSender;
	HCRYPTKEY_SimpleDeleter hAgreeKey;

	CreateAgreeKey(myCertSubjectString, responderCertSubjectString, &*hProvSender, &*hAgreeKey);

	//������������ ��� ��������� ����, ������ ��� ������ ������������ (???)
	if (!(ExportKey(*_hSessionKey,	*hAgreeKey, SIMPLEBLOB, publicKeyBlob, (DWORD*)blobSize)))
	{
		throw CryptoException("Can't export session key");
	}
	
}

//����������� ���������� ����, ������������� �����������
//(�����) �������� ������
void CryptoAPI::ImportSessionKey(uint8_t* key, uint32_t keySize, std::string myCertSubjectString, std::string senderCertSubjectString)
{
	HCRYPTPROV_SimpleDeleter hProv;
	HCRYPTKEY_SimpleDeleter hAgreeKey;

	CreateAgreeKey(myCertSubjectString, senderCertSubjectString, &*hProv, &*hAgreeKey);

	_sessionKeyMutex.lock();

	if (!CryptImportKey(*hProv, key, keySize, *hAgreeKey, 0,&*_hSessionKey))
	{
		_sessionKeyMutex.unlock();
		throw CryptoException("Can't import session key");
	}

	_sessionKeyMutex.unlock();
}

/*!
 * \brief ������������ ���� ���������� ��� ���������� ����� � ������� x509 DER
 */
void Crypto::CryptoAPI::ExportMyCertificate(std::string myCertSubjectString, uint8_t ** certBuffer, uint32_t * bufferLength)
{
	PCCERT_CONTEXT_SimpleDeleter myCert;
	*myCert = FindCertificate(CERT_PERSONAL_STORE, myCertSubjectString);

	*bufferLength = (*myCert)->cbCertEncoded;
	*certBuffer = new uint8_t[*bufferLength];
	memcpy(*certBuffer, (*myCert)->pbCertEncoded, *bufferLength);
}

//������� ������ � �������������� ����������� �����
void CryptoAPI::Encrypt(uint8_t* data, uint32_t size)
{
	DWORD dataLen = size;

	if (!CryptSetKeyParam(*_hSessionKey, KP_IV, (const BYTE*)_keyParams, 0))
	{
		DWORD error = GetLastError();
		Util::Logger::Instance()->Write(QString("Can't set key params. Error: %1").arg(error, 0, 16));
		//throw  CryptoException(QString("Can't set key params. Error: %1").arg(error, 0, 16));
		//throw CryptoException("Can't set key params");
	}

	if (!CryptEncrypt(*_hSessionKey, NULL, true, NULL, data, &dataLen, size))
	{
		DWORD error = GetLastError();
		Util::Logger::Instance()->Write(QString("Can't encrypt data. Error: %1").arg(error, 0, 16));
		//throw  CryptoException(QString("Can't encrypt data. Error: %1").arg(error, 0, 16));
		//throw CryptoException("Can't encrypt data");
	}
}

//�������������� ������ � �������������� ����������� �����

void CryptoAPI::Decrypt(uint8_t* data, uint32_t size)
{
	DWORD dataLen = size;

	if (!CryptSetKeyParam(*_hSessionKey, KP_IV, (const BYTE*)_keyParams, 0))
	{
		DWORD error = GetLastError();
		Util::Logger::Instance()->Write(QString("Can't set key params. Error: %1").arg(error, 0, 16));
		//throw  CryptoException(QString("Can't set key params. Error: %1").arg(error,0,16));
		//throw CryptoException("Can't set key params");
	}

	if (!CryptDecrypt(*_hSessionKey, NULL, true, NULL, data, &dataLen))
	{
		DWORD error = GetLastError();
		Util::Logger::Instance()->Write(QString("Can't decrypt data. Error: %1").arg(error, 0, 16));
		//throw  CryptoException(QString("Can't decrypt data. Error: %1").arg(error, 0, 16));
		//throw CryptoException("Can't decrypt data.");
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////


//��������� ��������� ������������
HCERTSTORE CryptoAPI::OpenCertStore(std::string storeName)
{
	/*if (!_hCryptProv.IsValid())
		throw CryptoException("Crypto provider is not initialized");*/

	HCERTSTORE hCertStore = CertOpenSystemStoreA(*_hCryptProv, storeName.c_str());
	if (!hCertStore)
	{
		//throw CryptoException(std::string("Can't open system store \"") + std::string(storeName) + std::string("\""));
		return NULL;
	}

	return hCertStore;
}

//��������� ��������� ������������
void CryptoAPI::CloseCertStore(HCERTSTORE hCertStore)
{
	if (hCertStore)
		if (!CertCloseStore(hCertStore, CERT_CLOSE_STORE_CHECK_FLAG))
			throw CryptoException("Can't close certificate store");

}


//�������� ���������� �� ���������
PCCERT_CONTEXT Crypto::CryptoAPI::FindCertificate(std::string storeName, std::string certName)
{
	//�������� ������� ��������� ������������
	HCERTSTORE_SimpleDeleter hStore;
	*hStore = OpenCertStore(storeName);

	if (*hStore == NULL)
		throw CryptoException("Can't open certificate store");

	PCCERT_CONTEXT hCert;
	hCert = CertFindCertificateInStore(
		*hStore,
		_dwCertEncodingType,
		0,
		CERT_FIND_SUBJECT_STR,
		certName.c_str(),
		NULL
	);

	if (hCert == NULL)
		//throw CryptoException(QString("Certificate \"%1\" not found").arg(certName.c_str()));
		throw CryptoException("Certificate \"%1\" not found");

	return hCert;

}

/*!
* \brief ������������ ����
*
* param[in] keyToExport ����, ������� ��������������
* param[in] keyToEncode ����, ������������ ��� ���������� ����� (��� NULL)
* param[in] blobType    ��� ����� ��� ��������
* param[out] keyBlob    ����� � ������
* param[out] keySize    ������ ������
*
* \return ����� ��������
*/
bool CryptoAPI::ExportKey(HCRYPTKEY keyToExport, HCRYPTKEY keyToEncode, DWORD blobType, BYTE** keyBlob, DWORD* keySize)
{
	if (!CryptExportKey(
		keyToExport,
		keyToEncode,
		blobType,
		NULL,
		NULL,
		keySize
	))
		return false;
	
	*keyBlob = new BYTE[*keySize];

	if (!CryptExportKey(
		keyToExport,
		keyToEncode,
		blobType,
		NULL,
		*keyBlob,
		keySize
	))
	{
		*keySize = -1;
		keyBlob = NULL;
		return false;
	}

	return true;
}



/*
������� ���� ������������

��������� ����������� ��� �������� �� ���������� � �������\�������� ����������� �����.
��������� �����������:

1. ������� ��� ����������
2. �������� ��� CSP
3. ��������� ����������� ������ ��������� �����

4. ����� ����������� ������� ��������� (����������/�����������)
5. ��������� ��� ���������� �����
6. ������� � ���� ��� ���������� �����

7. ��������� ����� ������������ �� ������ ���������� � ��� ���������� �����

*/

void CryptoAPI::CreateAgreeKey(std::string myCertSubjectString, std::string otherCertSubjectString, HCRYPTPROV* hProv, HCRYPTKEY* hAgreeKey)
{
	

	DWORD dwKeySpecSender;

	//�������� ���� ���������
	PCCERT_CONTEXT_SimpleDeleter hMyCert;

	*hMyCert = FindCertificate(CERT_PERSONAL_STORE, myCertSubjectString);

	// ��������� ����������� CSP, ������� ������ � ���������� � ��� ���������
	// ���������� ��� ��������� ����������� pCertSender.
	if (!CryptAcquireCertificatePrivateKey(
		*hMyCert,
		0,
		NULL,
		hProv,
		&dwKeySpecSender,
		NULL
	))
	{
		throw CryptoException("Can't accuire certificate private key");
	}

	//��������� ����������� ������ ��������� �����
	HCRYPTKEY_SimpleDeleter hMyPrivateKey;

	if (!CryptGetUserKey(
		*hProv,
		dwKeySpecSender,
		&*hMyPrivateKey
	))
	{
		throw CryptoException("Can't get own private key");
	}

	//���� ���������� ������ ������� (���� �� �������� - ����������, ���� �� ��������� - �����������)
	PCCERT_CONTEXT_SimpleDeleter hResponderCert;

	*hResponderCert = FindCertificate(CERT_OTHERS_STORE, otherCertSubjectString);

	//�������� ��������� ���� ������ �������
	HCRYPTKEY_SimpleDeleter hPublicKey;

	if (!CryptImportPublicKeyInfoEx(
		*_hCryptProv,
		_dwCertEncodingType,
		&(*hResponderCert)->pCertInfo->SubjectPublicKeyInfo,
		0,
		0,
		NULL,
		&*hPublicKey))
	{
		throw CryptoException("Can't import public key from certificate");
	}

	//������������ ��������� ���� ������ �������
	DWORD dwPublicKeyBlobLen;
	BYTE* publicKeyBlob;

	if (!ExportKey(
		*hPublicKey,
		NULL,
		PUBLICKEYBLOB,
		&publicKeyBlob,
		&dwPublicKeyBlobLen
	))
	{
		throw CryptoException("Can't export other's public key");
	}

	//�������� ���� ������������
	if (!CryptImportKey(
		*hProv,
		publicKeyBlob,
		dwPublicKeyBlobLen,
		*hMyPrivateKey,
		0,
		hAgreeKey
	))
	{
		delete[] publicKeyBlob;
		throw CryptoException("Can't get agree key");
	}

	delete[] publicKeyBlob;
}




std::string CryptoAPI::ErrorToString(DWORD error)
{
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			LocalFree(lpMsgBuf);

			return result;
		}
	}
	return std::string();
}
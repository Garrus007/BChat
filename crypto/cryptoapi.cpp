#include "cryptoapi.h"

using namespace Crypto;

CryptoAPI::CryptoAPI(const char* containerName):_sessionKeyMutex()
{

	//�������� ���������
	if (!CryptAcquireContextA(
		&*_hCryptProv,
		containerName,			//��� ����������
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

		throw CryptoException(QString("Cryptographic context with container \"%1\" couldn't be accured").arg(containerName));
	}
}


CryptoAPI::~CryptoAPI()
{
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
bool CryptoAPI::ExportSessionKeyForUser(std::string myCertSubjectString, std::string responderCertSubjectString)
{
	PCCERT_CONTEXT_SimpleDeleter hMyCert;
	PCCERT_CONTEXT_SimpleDeleter hResponderCert;

	HCRYPTPROV_SimpleDeleter hProvSender;
	HCRYPTKEY_SimpleDeleter hMyPrivateKey;
	DWORD dwKeySpecSender;

	HCRYPTKEY_SimpleDeleter hPublicKey;
	DWORD dwPublicKeyBlobLen;
	BYTE* publicKeyBlob;

	HCRYPTKEY_SimpleDeleter hAgreeKey;

	BYTE* sessionKeyBlob;
	DWORD dwSessionKeyBlobLen;

	//�������� ���� ���������
	
	*hMyCert =  FindCertificate(CERT_PERSONAL_STORE, myCertSubjectString);

	// ��������� ����������� CSP, ������� ������ � ���������� � ��� ���������
	// ���������� ��� ��������� ����������� pCertSender.
	if (!CryptAcquireCertificatePrivateKey(
		*hMyCert,
		0,
		NULL,
		&*hProvSender,
		&dwKeySpecSender,
		NULL
	))
	{
		throw new CryptoException("Can't accuire certificate private key");
	}

	//��������� ����������� ��������� ����� ����������
	if (!CryptGetUserKey(
		*hProvSender,
		dwKeySpecSender,
		&*hMyPrivateKey
	))
	{
		throw new CryptoException("Can't get own private key");
	}

	//���� ���������� ����������
	
	*hResponderCert = FindCertificate(CERT_OTHERS_STORE, responderCertSubjectString);

	//�������� ��������� ���� ����������
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


	//������������ ��������� ���� ����������

	
	if (!ExportKey(
		*hPublicKey,
		NULL,
		PUBLICKEYBLOB,
		&publicKeyBlob,
		&dwPublicKeyBlobLen
	))
	{
		throw new CryptoException("Can't export responder public key");
	}

	//�������� ���� ������������
	if (!CryptImportKey(
		*hProvSender,
		publicKeyBlob,
		dwPublicKeyBlobLen,
		*hMyPrivateKey,
		0,
		&*hAgreeKey
	))
	{
		throw new CryptoException("Can't get agree key");
	}

	//������������ ��� ��������� ����, ������ ��� ������ ������������ (???)

	if (!(ExportKey(
		*_hSessionKey,
		*hAgreeKey,
		SIMPLEBLOB,
		&sessionKeyBlob,
		&dwSessionKeyBlobLen
	)))
	{
		throw new CryptoException("Can't export session key");
	}
	
}

//����������� ���������� ����, ������������� �����������
//(�����) �������� ������
bool CryptoAPI::ImportSessionKey(uint8_t* key, uint32_t keySize)
{
	throw NotImplementedException();
}



//������� ������ � �������������� ����������� �����
bool CryptoAPI::Encrypt(uint8_t* data, uint32_t size)
{
	throw NotImplementedException();
}

//�������������� ������ � �������������� ����������� �����

bool CryptoAPI::Decrypt(uint8_t* data, uint32_t size)
{
	throw NotImplementedException();
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
		//throw new CryptoException(std::string("Can't open system store \"") + std::string(storeName) + std::string("\""));
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
		throw new CryptoException(QString("Sertificate \"%1\" not found").arg(certName.c_str()));

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
#include "cryptoapi.h"

using namespace Crypto;

CryptoAPI::CryptoAPI(const char* containerName):_sessionKeyMutex()
{
	_hCryptProv = nullptr;
	_hSessionKey = nullptr;

	HCRYPTPROV hCryptProv;
	HCRYPTKEY hCryptKey;

	//�������� ���������
	if (!CryptAcquireContextA(
		&hCryptProv,
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


		throw CryptoException(std::string("Cryptographic context with container \"")
			+ std::string(containerName)
			+ std::string(+"\" couldn't be accured"));
	}

	_hCryptProv = std::unique_ptr<HCRYPTPROV, HCRYPTPROV_Deleter>(&hCryptProv);
}


CryptoAPI::~CryptoAPI()
{
	CleanUp();
}





///////////////////////////////////////////////////////////////////////////////////////////////////


//���������� ���������� ����
bool CryptoAPI::CreateSessionKey()
{	
	HCRYPTKEY hSessionKey;

	_sessionKeyMutex.lock();
	bool result =  CryptGenKey(*_hCryptProv, CALG_G28147, CRYPT_EXPORTABLE, &hSessionKey);
	_hSessionKey = std::unique_ptr<HCRYPTKEY, HCRYPTKEY_Deleter>(&hSessionKey);
	_sessionKeyMutex.unlock();

	return result;
}


//������������ ���������� ���� � ������� �������� ������
//���������������� ������������
bool CryptoAPI::ExportSessionKeyForUser(std::string certSerialNumber)
{
	if (_hCryptProv == nullptr)
		throw CryptoException("Crypto provider is not initialized");

	if (_hSessionKey == nullptr)
		throw CryptoException("Session key does not exists");

	//��������� ��������� ������������ ������ �������������
	HCERTSTORE _hCertStore = OpenCertStore(CERT_OTHERS_STORE);
	std::unique_ptr<HCERTSTORE, HCERTSTORE_Deleter> hOtherStore(&_hCertStore);

	if (*hOtherStore == NULL)
	{
		throw CryptoException("Can't open certificate store");
	}

	//���� ���������� ����������
	PCCERT_CONTEXT _certContext = CertFindCertificateInStore(*hOtherStore, _dwCertEncodingType, 0, CERT_FIND_SUBJECT_STR, certSerialNumber.c_str(), NULL);
	std::unique_ptr<PCCERT_CONTEXT, PCCERT_CONTEXT_Deleter> certContext(&_certContext);
	if (certContext == NULL)
	{
		throw CryptoException("User's certificate not found");
	}

	HCRYPTPROV prov = *_hCryptProv;
	
	//�������� ����
	HCRYPTKEY  _hPublicKey;
	if (!CryptImportPublicKeyInfo(
		prov,
		_dwCertEncodingType,
		&_certContext->pCertInfo->SubjectPublicKeyInfo,
		&_hPublicKey))
	{
		throw CryptoException("Can't import public key from certificate");
	}
	std::unique_ptr<HCRYPTKEY, HCRYPTKEY_Deleter> hPublicKey(&_hPublicKey);

	//������������ ��� ��������� ����, ������ ��� �������� ������ ����������
	DWORD dwBlobLen;
	BYTE* sessionKeyBlob;

	//������� - ������, ����� ��� ����
	if (!CryptExportKey(
		*_hSessionKey,
		NULL,
		PLAINTEXTKEYBLOB,
		0,
		NULL,
		&dwBlobLen
	))
	{
		throw CryptoException("Can't export session key");
	}

	//�������� ������ ��� BLOB ����������� �����
	sessionKeyBlob = new BYTE[dwBlobLen];

	//������������ ��� ����
	if (!CryptExportKey(
		*_hSessionKey,
		*hPublicKey,
		SIMPLEBLOB,
		0,
		sessionKeyBlob,
		&dwBlobLen
	))
	{
		throw CryptoException("Can't export session key");
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

//��������� � ������� ��� �����������
void CryptoAPI::CleanUp()
{

	/*if (_hCryptProv)
	if (!CryptReleaseContext(_hCryptProv, NULL))
	throw new CryptoException("Cant' close crypto provider");*/
}


//��������� ��������� ������������
HCERTSTORE CryptoAPI::OpenCertStore(std::string storeName)
{
	if (_hCryptProv == nullptr)
		throw CryptoException("Crypto provider is not initialized");

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
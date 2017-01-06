#include "cryptoapi.h"

CryptoAPI::CryptoAPI(const char* containerName):_sessionKeyMutex()
{

	//�������� ���������
	if (!CryptAcquireContextA(
		&_hCryptProv,
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
}


CryptoAPI::~CryptoAPI()
{
	CleanUp();
}



//��������� � ������� ��� �����������
void CryptoAPI::CleanUp()
{
	
	if (_hCryptProv)
		if (!CryptReleaseContext(_hCryptProv, NULL))
			throw new CryptoException("Cant' close crypto provider");
}


//.................................................................................................



//��������� ��������� ������������
HCERTSTORE CryptoAPI::OpenCertStore(std::string storeName)
{
	HCERTSTORE hCertStore = CertOpenSystemStoreA(_hCryptProv, storeName.c_str());
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
			throw new CryptoException("Can't close certificate store");

}

//���������� ���������� ����
bool CryptoAPI::CreateSessionKey()
{
	_sessionKeyMutex.lock();
	bool result =  CryptGenKey(_hCryptProv, CALG_G28147, CRYPT_EXPORTABLE, &_hSessionKey);
	_sessionKeyMutex.unlock();

	return result;
}


//������������ ���������� ���� � ������� �������� ������
//���������������� ������������
bool CryptoAPI::ExportSessionKeyForUser(std::string certSerialNumber)
{
	//��������� ��������� ������������ ������ �������������
	HCERTSTORE _hCertStore = OpenCertStore(CERT_OTHERS_STORE);
	std::unique_ptr<HCERTSTORE, HCERTSTORE_Deleter> hOtherStore(&_hCertStore);

	if (*hOtherStore == NULL)
	{
		throw new CryptoException("Can't open certificate store");
	}

	//���� ����������
	PCCERT_CONTEXT _certContext = CertFindCertificateInStore(*hOtherStore, _dwCertEncodingType, 0, CERT_FIND_SUBJECT_STR, certSerialNumber.c_str(), NULL);
	std::unique_ptr<PCCERT_CONTEXT, PCCERT_CONTEXT_Deleter> certContext(&_certContext);
	if (certContext == NULL)
	{
		throw new CryptoException("User's certificate not found");
	}

	//�������� ����
	HCRYPTKEY  _hPublicKey;
	if (!CryptImportPublicKeyInfo(
		_hCryptProv,
		_dwCertEncodingType,
		&(*certContext)->pCertInfo->SubjectPublicKeyInfo,
		&_hPublicKey))
	{
		throw new CryptoException("Can't import public key from certificate");
	}
	std::unique_ptr<HCRYPTKEY, HCRYPTKEY_Deleter> hPublicKey(&_hPublicKey);

	//������������ ��� ��������� ����, ������ ��� �������� ������ ����������
	DWORD dwBlobLen;

	//������� - ������, ����� ��� ����
	if (!CryptExportKey(
		_hSessionKey,
		*hPublicKey,
		SIMPLEBLOB,
		0,
		NULL,
		&dwBlobLen
	))
	{
		throw new CryptoException("Can't export session key");
	}

	//CryptDestroyKey(hPublicKey);
	//CertFreeCertificateContext(certContext);
	//CloseCertStore(hOtherStore);
}

//����������� ���������� ����, ������������� �����������
//(�����) �������� ������
bool CryptoAPI::ImportSessionKey(uint8_t* key, uint32_t keySize)
{
	throw new NotImplementedException();
}



//������� ������ � �������������� ����������� �����
bool CryptoAPI::Encrypt(uint8_t* data, uint32_t size)
{
	throw new NotImplementedException();
}

//�������������� ������ � �������������� ����������� �����

bool CryptoAPI::Decrypt(uint8_t* data, uint32_t size)
{
	throw new NotImplementedException();
}


//...................... ������� �������� �������� WINAPI .........................................

void CryptoAPI::CloseCyrptoProv(HCRYPTPROV* hCryptProv)
{

}

void CryptoAPI::CloseKey(HCRYPTKEY* hKey)
{

}

void CryptoAPI::CloseCertStore(HCERTSTORE* hCertStore)
{

}
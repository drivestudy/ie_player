// file_encrypt_and_dencrypt.cpp : �������̨Ӧ�ó������ڵ㡣
//

/////////////////////////////////////////////////////////////
// ˵����
// �ӽ��ܺ��������ܲ��ԡ�
// ������Ƶ������ʹ��AES����Ϊ����ͬ����
// �������Ƶ���ִ�����ôʹ�ÿ��ٵ�RC4
/////////////////////////////////////////////////////////////
// ͼƬѡ�����AES����ʽ����16�ֽڷֿ�
// Ҫ������ȫ�������AES���ܣ�RSA����Կ����Կ��˽Կ�ı���
/////////////////////////////////////////////////////////////



#include "stdafx.h"
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/rc4.h>
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#include "iostream"
using namespace std;
#include "string"
#include "fstream"

#define AES_KEY_BUF_LENGTH 128

#define RELESE(P) if (P)		\
{								\
	delete P;					\
	P = NULL;					\
}

#define RELESE_ARRAY(P) if (P)  \
{								\
	delete[] P;					\
	P = NULL;					\
}


// ����ʹ��aes�����㷨������
void TestAesEncrypt()
{
	unsigned char buf[16];
	memset(buf,1,sizeof(buf));
	strcpy((char *)buf, "zengraoli");

	cout << "current buf value is :" << buf << endl;

	unsigned char buf2[16];
	unsigned char buf3[16];
	unsigned char aes_keybuf[AES_KEY_BUF_LENGTH];

	memset(aes_keybuf,0,sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, "zeng");
	cout << "current aes_keybuf value is :" << aes_keybuf << endl;
	AES_KEY aeskey;

	AES_set_encrypt_key(aes_keybuf,256,&aeskey);
	AES_encrypt(buf,buf2,&aeskey);

	cout << "current buf2 value is :" << buf2 << endl;

	memset(aes_keybuf,0,sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, "zeng2");
	cout << "current aes_keybuf value is :" << aes_keybuf << endl;
	AES_set_decrypt_key(aes_keybuf,256,&aeskey);

	AES_decrypt(buf2,buf3,&aeskey);

	cout << "current buf3 value is :" << buf3 << endl;

	if(memcmp(buf,buf3,sizeof(buf))==0)
		printf("test success\r\n");
	else
		printf("test fail\r\n");
}


// ����ʹ��aes�����ļ��㷨������
int TestAesEncryptFile(std::string in_file_path, std::string out_file_path, 
					   const char *aes_encrypt_key, int in_data_chunk_size = 16)
{
	ifstream fin(in_file_path.c_str(), ios::binary);
	ofstream fout(out_file_path, ios::binary);

	if(!fin)
	{
		cout << "Can not open fin file." << endl;
		return 1;
	}
	if(!fout)
	{
		cout << "Can not open fout file." << endl;
		return 1;
	}

	//��ָ����Կ��һ���ڴ���м��ܣ��������outbuffer��
	unsigned char aes_keybuf[AES_KEY_BUF_LENGTH];
	memset(aes_keybuf,0,sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, aes_encrypt_key);
	AES_KEY aeskey;
	AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

	char *in_data  = new char[in_data_chunk_size + 1];
	char *out_data = new char[in_data_chunk_size + 1];
	while(!fin.eof())
	{
		fin.read(in_data, in_data_chunk_size);
		AES_encrypt((const unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
		fout.write(out_data, fin.gcount());
	};

	fout.close();
	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(out_data);

	return 0;
}

// ����ʹ��aes�����ļ��㷨������
int TestAesDecryptFile(std::string in_file_path, std::string out_file_path, 
					   const char *rc4_dencrypt_key, int in_data_chunk_size = 16)
{
	ifstream fin(in_file_path.c_str(), ios::binary);
	ofstream fout(out_file_path, ios::binary);

	if(!fin)
	{
		cout << "Can not open fin file." << endl;
		return 1;
	}
	if(!fout)
	{
		cout << "Can not open fout file." << endl;
		return 1;
	}

	//��ָ����Կ��һ���ڴ���м��ܣ��������outbuffer��
	unsigned char aes_keybuf[AES_KEY_BUF_LENGTH];
	memset(aes_keybuf,0,sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, rc4_dencrypt_key);
	AES_KEY aeskey;
	AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

	char *in_data  = new char[in_data_chunk_size + 1];
	char *out_data = new char[in_data_chunk_size + 1];
	while( ! fin.eof() )
	{
		fin.read(in_data, in_data_chunk_size);
		AES_decrypt((unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
		fout.write(out_data, fin.gcount());
	};

	fout.close();
	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(out_data);

	return 0;
}


// ���������in_data�ֽڴ�С����aes���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
int AesEncryptBytes(const unsigned char* in_data, unsigned char* out_data, 
					const char* aes_encrypt_key, int in_data_chunk_size = 16)
{
	if (strlen((char *)aes_encrypt_key) == 0)
	{
		return 1;
	}

	unsigned char aes_keybuf[AES_KEY_BUF_LENGTH];
	memset(aes_keybuf, 0, sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, aes_encrypt_key);
	AES_KEY aeskey;
	AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

	AES_encrypt(in_data, out_data, &aeskey);

	return 0;
}


// ���������in_data�ֽڴ�С����aes���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
int AesDecryptBytes(const unsigned char* in_data, unsigned char* out_data, 
					const char* aes_decrypt_key, int in_data_chunk_size = 16)
{
	if (strlen((char *)aes_decrypt_key) == 0)
	{
		return 1;
	}

	unsigned char aes_keybuf[AES_KEY_BUF_LENGTH];
	memset(aes_keybuf, 0, sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, aes_decrypt_key);
	AES_KEY aeskey;
	AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

	AES_decrypt(in_data, out_data, &aeskey);

	return 0;
}


// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
int AesEncryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data,
							  const char* aes_encrypt_key, int in_data_chunk_size = 16)
{
	ifstream fin(in_file_path, ios::binary);

	if (!fin)
	{
		return 1;
	}

	int out_data_position = 0;
	char *in_data  = new char[in_data_chunk_size + 1];
	char *in_data_tmp  = new char[in_data_chunk_size + 1];
	while (!fin.eof())
	{
		fin.read(in_data, in_data_chunk_size);
		AesEncryptBytes((const unsigned char*)in_data, (unsigned char*)in_data_tmp, aes_encrypt_key, in_data_chunk_size);

		for (int in_data_tmp_position = 0; in_data_tmp_position < fin.gcount(); in_data_tmp_position++)
		{
			out_data[out_data_position++] =  in_data_tmp[in_data_tmp_position];
		}
	}

	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(in_data_tmp);

	if (out_data_position == 0)
	{
		return 1;
	}
	
	return 0;
}


// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
int AesDecryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data,
							  const char* aes_encrypt_key, int in_data_chunk_size = 16)
{
	ifstream fin(in_file_path, ios::binary);

	ofstream fout("3.gif", ios::binary);

	if (!fin)
	{
		return 1;
	}

	int out_data_position = 0;
	char *in_data  = new char[in_data_chunk_size + 1];
	char *in_data_tmp  = new char[in_data_chunk_size + 1];
	while (!fin.eof())
	{
		fin.read(in_data, in_data_chunk_size);
		AesDecryptBytes((const unsigned char *)in_data, (unsigned char *)in_data_tmp, 
						aes_encrypt_key, in_data_chunk_size);
		for (int in_data_tmp_position = 0; in_data_tmp_position < fin.gcount(); in_data_tmp_position++)
		{
			out_data[out_data_position++] =  in_data_tmp[in_data_tmp_position];
		}
	}

	// ����д���ļ���
	fout.write((char *)out_data, out_data_position);

	fin.close();
	fout.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(in_data_tmp);

	if (out_data_position == 0)
	{
		return 1;
	}

	return 0;
}

// ����ʹ��rsa�����㷨������
void TestRsaEncrypt()
{
	BIGNUM b={0};
	RSA* pRsa = RSA_generate_key(1024, RSA_F4, 0, 0);

	//pRsa�а�����N D,�������Լ��޸ľͿ�����
	char in_data[] = "zengraoli";
	cout << "current in_data value is : " << in_data << endl;

	int len = RSA_size(pRsa);
	char* out_data = new char[len];
	memset(out_data, 0, len);
	RSA_public_encrypt( sizeof(in_data), (unsigned char *)in_data,
		(unsigned char *)out_data, pRsa, RSA_PKCS1_PADDING);
	cout << "current out_data value is : " << out_data << endl;

	char out[1024] = {0};
	RSA_private_decrypt(len, (unsigned char *)out_data, 
		(unsigned char *)out, pRsa, RSA_PKCS1_PADDING);
	RSA_free(pRsa);

	cout << "current out value is : " << out << endl;
}


// ����ʹ��rc4�����㷨������
void TestRc4Encrypt()
{
	char code[64]={0};
	int codelen = sizeof(code);
	memcpy_s(code, 64, "This is secrect", 15);

	cout << "before encrypt :" << code << endl;
	unsigned char *outbuffer = new unsigned char[codelen];

	//��ָ����Կ��һ���ڴ���м��ܣ��������outbuffer��
	RC4_KEY rc4_key;
	RC4_set_key(&rc4_key,AES_KEY_BUF_LENGTH,(unsigned char *)"zenraoli");
	RC4(&rc4_key,codelen,(unsigned char *)code,outbuffer);
	cout << "after encrypt :" << outbuffer << endl;

	//��ָ����Կ��outbuffer�е����Ľ��н��ܣ�����Ż�ԭ�����ڴ�
	memset(code,0,sizeof(code));
	RC4_set_key(&rc4_key,7,(unsigned char *)"zenraoli");//��������ٴ�������Կ
	RC4(&rc4_key,codelen,outbuffer,(unsigned char *)code);
	cout << "after decrypt :" << code << endl;

	if (outbuffer)
	{
		delete[] outbuffer;
		outbuffer = NULL;
	}
}


// ����ʹ��rc4�����ļ��㷨������
int TestRc4EncryptFile(std::string in_file_path, std::string out_file_path, 
					   const char *rc4_encrypt_key, int encrypt_chunk_size = 16)
{
	ifstream fin(in_file_path.c_str(), ios::binary);
	ofstream fout(out_file_path, ios::binary);

	if(!fin)
	{
		cout << "Can not open fin file." << endl;
		return 1;
	}
	if(!fout)
	{
		cout << "Can not open fout file." << endl;
		return 1;
	}

	//��ָ����Կ��һ���ڴ���м��ܣ��������outbuffer��
	char code[AES_KEY_BUF_LENGTH] = {0};
	int codelen = sizeof(code);
	RC4_KEY rc4_key;
	RC4_set_key(&rc4_key, strlen(rc4_encrypt_key), (unsigned char *)rc4_encrypt_key);

	char *in_data  = new char[encrypt_chunk_size + 1];
	char *out_data = new char[encrypt_chunk_size + 1];
	while(!fin.eof())
	{
		fin.read(in_data, encrypt_chunk_size);
		RC4(&rc4_key, (size_t)fin.gcount(),(unsigned char *)in_data, (unsigned char *)out_data);
		fout.write(out_data, fin.gcount());
	};

	fout.close();
	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(out_data);

	return 0;
}

// ����ʹ��rc4�����ļ��㷨������
int TestRc4DecryptFile(std::string in_file_path, std::string out_file_path, 
					   const char *rc4_dencrypt_key, int encrypt_chunk_size = 16)
{
	ifstream fin(in_file_path.c_str(), ios::binary);
	ofstream fout(out_file_path, ios::binary);

	if(!fin)
	{
		cout << "Can not open fin file." << endl;
		return 1;
	}
	if(!fout)
	{
		cout << "Can not open fout file." << endl;
		return 1;
	}

	//��ָ����Կ��һ���ڴ���м��ܣ��������outbuffer��
	char code[AES_KEY_BUF_LENGTH] = {0};
	int codelen = sizeof(code);
	RC4_KEY rc4_key;
	RC4_set_key(&rc4_key, strlen(rc4_dencrypt_key), (unsigned char *)rc4_dencrypt_key);

	char *in_data  = new char[encrypt_chunk_size + 1];
	char *out_data = new char[encrypt_chunk_size + 1];
	while(!fin.eof())
	{
		fin.read(in_data, encrypt_chunk_size);
		RC4(&rc4_key, (size_t)fin.gcount(),(unsigned char *)in_data, (unsigned char *)out_data);
		fout.write(out_data, fin.gcount());
	};

	fout.close();
	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(out_data);

	return 0;
}


// ���������in_data�ֽڴ�С����rc4���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
int Rc4EncryptBytes(const unsigned char* in_data, unsigned char* out_data, 
					const char* rc4_encrypt_key, int in_data_chunk_size = 16)
{
	if (strlen((char *)rc4_encrypt_key) == 0)
	{
		return 1;
	}

	RC4_KEY rc4_key;
	RC4_set_key(&rc4_key, strlen(rc4_encrypt_key), (unsigned char *)rc4_encrypt_key);

	RC4(&rc4_key, in_data_chunk_size, (unsigned char *)in_data, (unsigned char *)out_data);

	return 0;
}


// ���������in_data�ֽڴ�С����rc4���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
int Rc4DecryptBytes(const unsigned char* in_data, unsigned char* out_data, 
					const char* rc4_dencrypt_key, int in_data_chunk_size = 16)
{
	if (strlen((char *)rc4_dencrypt_key) == 0)
	{
		return 1;
	}

	RC4_KEY rc4_key;
	RC4_set_key(&rc4_key, strlen(rc4_dencrypt_key), (unsigned char *)rc4_dencrypt_key);

	RC4(&rc4_key, in_data_chunk_size, (unsigned char *)in_data, (unsigned char *)out_data);

	return 0;
}


// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
int Rc4EncryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data,
							  const char* aes_encrypt_key, int in_data_chunk_size = 16)
{
	ifstream fin(in_file_path, ios::binary);

	if (!fin)
	{
		return 1;
	}

	int out_data_position = 0;
	char *in_data  = new char[in_data_chunk_size + 1];
	char *in_data_tmp  = new char[in_data_chunk_size + 1];
	while (!fin.eof())
	{
		fin.read(in_data, in_data_chunk_size);
		Rc4EncryptBytes((const unsigned char*)in_data, (unsigned char*)in_data_tmp, aes_encrypt_key, in_data_chunk_size);

		for (int in_data_tmp_position = 0; in_data_tmp_position < fin.gcount(); in_data_tmp_position++)
		{
			out_data[out_data_position++] =  in_data_tmp[in_data_tmp_position];
		}
	}

	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(in_data_tmp);

	if (out_data_position == 0)
	{
		return 1;
	}

	return 0;
}


// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
int Rc4DecryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data,
							  const char* aes_encrypt_key, int in_data_chunk_size = 16)
{
	ifstream fin(in_file_path, ios::binary);

	ofstream fout("3.gif", ios::binary);

	if (!fin)
	{
		return 1;
	}

	int out_data_position = 0;
	char *in_data  = new char[in_data_chunk_size + 1];
	char *in_data_tmp  = new char[in_data_chunk_size + 1];
	while (!fin.eof())
	{
		fin.read(in_data, in_data_chunk_size);
		Rc4DecryptBytes((const unsigned char *)in_data, (unsigned char *)in_data_tmp, 
			aes_encrypt_key, in_data_chunk_size);
		for (int in_data_tmp_position = 0; in_data_tmp_position < fin.gcount(); in_data_tmp_position++)
		{
			out_data[out_data_position++] =  in_data_tmp[in_data_tmp_position];
		}
	}

	// ����д���ļ���
	fout.write((char *)out_data, out_data_position);

	fin.close();
	fout.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(in_data_tmp);

	if (out_data_position == 0)
	{
		return 1;
	}

	return 0;
}


int Test(char *in_data, char *out_data, char *aes_decrypt_key, int encrypt_chunk_size = 16)
{
	if (strlen((char *)aes_decrypt_key) == 0)
	{
		return 1;
	}

	unsigned char aes_keybuf[32];
	memset(aes_keybuf, 0, sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, aes_decrypt_key);
	AES_KEY aeskey;
	AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

// 	unsigned char *aes_keybuf = new unsigned char[strlen(aes_decrypt_key)];
// 	memset(aes_keybuf, 0, strlen(aes_decrypt_key));
// 	strcpy((char *)aes_keybuf, aes_decrypt_key);
// 	AES_KEY aeskey;
// 	AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

	AES_decrypt((unsigned char *)in_data, (unsigned char *)out_data, &aeskey);

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
// 	TestAesEncrypt();
// 
	TestAesEncryptFile("plane.png", "encrypt.png", "0123456789abcdef0123456789abcdee");

	TestAesDecryptFile("encrypt.png", "decrypt.png", "0123456789abcdef0123456789abcdee");

// 	TestRsaEncrypt();
// 
// 	TestRc4Encrypt();
// 
//	TestRc4EncryptFile("1.gif", "2.gif", "zengraoli");
// 
//	TestRc4DecryptFile("2.gif", "3.gif", "zengraoli");


	/*
		TestCase : AesEncryptBytes��AesDecryptBytes
	*/
	// AesEncryptBytes
// 	ifstream fin("1.gif", ios::binary);
// 	ofstream fout("2.gif", ios::binary);
// 
// 	int chunk_size = 16;
// 	char *in_data  = new char[chunk_size];
// 	char *out_data = new char[chunk_size];
// 
// 	while (!fin.eof())
// 	{
// 		fin.read(in_data, chunk_size);
// 		AesEncryptBytes((const unsigned char *)in_data, (unsigned char *)out_data, "0123456789abcdef0123456789abcdee");
// 		fout.write(out_data, fin.gcount());
// 	}
// 
// 	fin.close();
// 	fout.close();
// 
// 	RELESE_ARRAY(in_data);
// 	RELESE_ARRAY(out_data);
// 
// 	// AesDecryptBytes
// 	ifstream fin_dectypt("2.gif", ios::binary);
// 	ofstream fout_decypt("3.gif", ios::binary);
// 
// 	int chunk_size2 = 16;
// 	char *in_data2  = new char[chunk_size2];
// 	char *out_data2 = new char[chunk_size2];
// 
// 	while (!fin_dectypt.eof())
// 	{
// 		fin_dectypt.read(in_data2, chunk_size2);
// 		AesDecryptBytes((const unsigned char *)in_data2, (unsigned char *)out_data2, "0123456789abcdef0123456789abcdee");
// 		fout_decypt.write(out_data2, fin_dectypt.gcount());
// 	}
// 
// 	fin_dectypt.close();
// 	fout_decypt.close();
// 
// 	RELESE_ARRAY(in_data2);
// 	RELESE_ARRAY(out_data2);



	/*
		TestCase : AesEncryptFromFileToBytes��AesDecryptFromFileToBytes
	*/
	// AesEncryptFromFileToBytes
// 	ifstream fin_encrypt("1.gif", ios::binary);
// 	ofstream fout_encrypt("2.gif", ios::binary);
// 	fin_encrypt.seekg(0, ios::end);
// 	int file_length_encrypt = fin_encrypt.tellg();
// 	char *out_data_encrypt = new char[file_length_encrypt + 1];
// 	fin_encrypt.close();
// 	AesEncryptFromFileToBytes("1.gif", (unsigned char *)out_data_encrypt, "0123456789abcdef0123456789abcdee");
// 	fout_encrypt.write(out_data_encrypt, file_length_encrypt);
// 	RELESE_ARRAY(out_data_encrypt);
// 	fout_encrypt.close();
// 
// 	// AesDecryptFromFileToBytes
// 	ifstream fin_decrypt("2.gif", ios::binary);
// 	ofstream fout_decrypt("3.gif", ios::binary);
// 	fin_decrypt.seekg(0, ios::end);
// 	int file_decrypt_length = fin_decrypt.tellg();
// 	unsigned char *out_data_decrypt = new unsigned char[file_decrypt_length + 1];
// 	fin_decrypt.close();
// 	AesDecryptFromFileToBytes("2.gif", out_data_decrypt, "0123456789abcdef0123456789abcdee");
// 	fout_encrypt.write((const char *)out_data_decrypt, file_decrypt_length);
// 	fout_decrypt.close();
// 	RELESE_ARRAY(out_data_decrypt);



	/*
		TestCase : Rc4EncryptBytes��Rc4DecryptBytes
	*/
	// Rc4EncryptBytes
// 	ifstream fin("1.gif", ios::binary);
// 	ofstream fout("2.gif", ios::binary);
// 
// 	int chunk_size = 16;
// 	char *in_data  = new char[chunk_size];
// 	char *out_data = new char[chunk_size];
// 
// 	while (!fin.eof())
// 	{
// 		fin.read(in_data, chunk_size);
// 		Rc4EncryptBytes((const unsigned char *)in_data, (unsigned char *)out_data, "zengraoli");
// 		fout.write(out_data, fin.gcount());
// 	}
// 
// 	fin.close();
// 	fout.close();
// 
// 	RELESE_ARRAY(in_data);
// 	RELESE_ARRAY(out_data);
// 
// 	// Rc4DecryptBytes
// 	ifstream fin_dectypt("2.gif", ios::binary);
// 	ofstream fout_decypt("3.gif", ios::binary);
// 
// 	int chunk_size2 = 16;
// 	char *in_data2  = new char[chunk_size2];
// 	char *out_data2 = new char[chunk_size2];
// 
// 	while (!fin_dectypt.eof())
// 	{
// 		fin_dectypt.read(in_data2, chunk_size2);
// 		Rc4DecryptBytes((const unsigned char *)in_data2, (unsigned char *)out_data2, "zengraoli");
// 		fout_decypt.write(out_data2, fin_dectypt.gcount());
// 	}
// 
// 	fin_dectypt.close();
// 	fout_decypt.close();
// 
// 	RELESE_ARRAY(in_data2);
// 	RELESE_ARRAY(out_data2);


	/*
		TestCase : Rc4EncryptFromFileToBytes��Rc4DecryptFromFileToBytes
	*/
	// Rc4EncryptFromFileToBytes
// 	ifstream fin_encrypt("1.gif", ios::binary);
// 	ofstream fout_encrypt("2.gif", ios::binary);
// 	fin_encrypt.seekg(0, ios::end);
// 	int file_length_encrypt = fin_encrypt.tellg();
// 	char *out_data_encrypt = new char[file_length_encrypt + 1];
// 	fin_encrypt.close();
// 	Rc4EncryptFromFileToBytes("1.gif", (unsigned char *)out_data_encrypt, "zengraoli");
// 	fout_encrypt.write(out_data_encrypt, file_length_encrypt);
// 	RELESE_ARRAY(out_data_encrypt);
// 	fout_encrypt.close();
// 
// 	// Rc4DecryptFromFileToBytes
// 	ifstream fin_decrypt("2.gif", ios::binary);
// 	ofstream fout_decrypt("3.gif", ios::binary);
// 	fin_decrypt.seekg(0, ios::end);
// 	int file_decrypt_length = fin_decrypt.tellg();
// 	unsigned char *out_data_decrypt = new unsigned char[file_decrypt_length + 1];
// 	fin_decrypt.close();
// 	Rc4DecryptFromFileToBytes("2.gif", out_data_decrypt, "zengraoli");
// 	fout_encrypt.write((const char *)out_data_decrypt, file_decrypt_length);
// 	fout_decrypt.close();
// 	RELESE_ARRAY(out_data_decrypt);

	return 0;
}

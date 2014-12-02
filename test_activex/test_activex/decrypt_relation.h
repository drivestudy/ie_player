#ifndef  __DECRYPT_RELATION_H__
#define  __DECRYPT_RELATION_H__


#include "iostream"
#include "fstream"
#include "string"
using namespace std;

#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/rc4.h>

#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")


#define BUFFERSIZE  4096 * 4

#define AES_KEY_BUF_LENGTH 128 // ��Կ�ĳ�������

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


class DecryptRelation
{
public:

	// ����ʹ��aes�����㷨������
	static void AesEncrypt();

	// ����ʹ��aes�����ļ��㷨������
	static int AesEncryptFile(std::string in_file_path, std::string out_file_path, 
		const char *rc4_encrypt_key, int in_data_chunk_size = 16);

	// ����ʹ��aes�����ļ��㷨������
	static int AesDecryptFile(std::string in_file_path, std::string out_file_path, 
		const char *rc4_dencrypt_key, int in_data_chunk_size = 16);

	// ���������in_data�ֽڴ�С����aes���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
	static int AesEncryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// ���������in_data�ֽڴ�С����aes���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
	static int AesDecryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* aes_decrypt_key, int in_data_chunk_size = 16);

	// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
	static int AesEncryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
	static int AesDecryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// ����ʹ��rsa�����㷨������
	static void RsaEncrypt();

	// ����ʹ��rc4�����㷨������
	static void Rc4Encrypt();

	// ����ʹ��rc4�����ļ��㷨������
	static int Rc4EncryptFile(std::string in_file_path, std::string out_file_path,
		const char *rc4_encrypt_key, int encrypt_chunk_size = 16);

	// ����ʹ��rc4�����ļ��㷨������
	static int Rc4DecryptFile(std::string in_file_path, std::string out_file_path,
		const char *rc4_dencrypt_key, int encrypt_chunk_size = 16);

	// ���������in_data�ֽڴ�С����rc4���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
	static int Rc4EncryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* rc4_encrypt_key, int in_data_chunk_size = 16);

	// ���������in_data�ֽڴ�С����rc4���ܣ�����ou_data�ĳ��ȼ�⣬ȷ�����õ�ʱ�򳤶Ⱥ���
	static int Rc4DecryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* rc4_dencrypt_key, int in_data_chunk_size = 16);

	// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
	static int Rc4EncryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// ��in_file_path�ļ��н����ֽ����ݵ�out_data�У��������ȵ��жϣ����÷�����������out_data���ȿ���
	static int Rc4DecryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);


private:

	DecryptRelation();
	~DecryptRelation();
};


#endif // end if __DECRYPT_RELATION_H__
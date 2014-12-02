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

#define AES_KEY_BUF_LENGTH 128 // 密钥的长度设置

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

	// 测试使用aes加密算法的例子
	static void AesEncrypt();

	// 测试使用aes加密文件算法的例子
	static int AesEncryptFile(std::string in_file_path, std::string out_file_path, 
		const char *rc4_encrypt_key, int in_data_chunk_size = 16);

	// 测试使用aes解密文件算法的例子
	static int AesDecryptFile(std::string in_file_path, std::string out_file_path, 
		const char *rc4_dencrypt_key, int in_data_chunk_size = 16);

	// 对所输入的in_data字节大小进行aes加密，不做ou_data的长度检测，确保调用的时候长度合理
	static int AesEncryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// 对所输入的in_data字节大小进行aes解密，不做ou_data的长度检测，确保调用的时候长度合理
	static int AesDecryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* aes_decrypt_key, int in_data_chunk_size = 16);

	// 从in_file_path文件中解密字节数据到out_data中，不做长度的判断，调用方法必须做好out_data长度控制
	static int AesEncryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// 从in_file_path文件中解密字节数据到out_data中，不做长度的判断，调用方法必须做好out_data长度控制
	static int AesDecryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// 测试使用rsa加密算法的例子
	static void RsaEncrypt();

	// 测试使用rc4加密算法的例子
	static void Rc4Encrypt();

	// 测试使用rc4加密文件算法的例子
	static int Rc4EncryptFile(std::string in_file_path, std::string out_file_path,
		const char *rc4_encrypt_key, int encrypt_chunk_size = 16);

	// 测试使用rc4解密文件算法的例子
	static int Rc4DecryptFile(std::string in_file_path, std::string out_file_path,
		const char *rc4_dencrypt_key, int encrypt_chunk_size = 16);

	// 对所输入的in_data字节大小进行rc4加密，不做ou_data的长度检测，确保调用的时候长度合理
	static int Rc4EncryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* rc4_encrypt_key, int in_data_chunk_size = 16);

	// 对所输入的in_data字节大小进行rc4解密，不做ou_data的长度检测，确保调用的时候长度合理
	static int Rc4DecryptBytes(const unsigned char* in_data, unsigned char* out_data, 
		const char* rc4_dencrypt_key, int in_data_chunk_size = 16);

	// 从in_file_path文件中解密字节数据到out_data中，不做长度的判断，调用方法必须做好out_data长度控制
	static int Rc4EncryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);

	// 从in_file_path文件中解密字节数据到out_data中，不做长度的判断，调用方法必须做好out_data长度控制
	static int Rc4DecryptFromFileToBytes(const std::string in_file_path, unsigned char *out_data, int& out_data_length,
		const char* aes_encrypt_key, int in_data_chunk_size = 16);


private:

	DecryptRelation();
	~DecryptRelation();
};


#endif // end if __DECRYPT_RELATION_H__
#ifndef  __IIMAGE_DECRYPT_SHOW_H__
#define  __IIMAGE_DECRYPT_SHOW_H__


#include "iostream"
#include "string"


using namespace std;


class IImageDecryptShow
{
public:
	virtual inline CImage &GetImage() = 0;

	virtual inline void SetIsAlreadyDownLoad(bool is_already_down_load) = 0;

	virtual inline bool GetIsAlreadyDownLoad() = 0;

	// file_path : Ϊ�������ܴ����ͼ���ļ���·����Ϣ
	// decrypt_key : ������Կ
	// file_certificate_path : Ȩ��֤��·��
	virtual void ParaseImageAndShow(std::string file_path, std::string decrypt_key) = 0;

private:
	// ��out_data�ж�ȡ��ͼƬ���ݵ��ڴ��д���
	virtual	void LoadMemImage(void *out_data, long out_data_length) = 0;
};


#endif
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

	// file_path : 为经过加密处理后图像文件的路径信息
	// decrypt_key : 解密密钥
	// file_certificate_path : 权力证书路径
	virtual void ParaseImageAndShow(std::string file_path, std::string decrypt_key) = 0;

private:
	// 从out_data中读取出图片数据到内存中处理
	virtual	void LoadMemImage(void *out_data, long out_data_length) = 0;
};


#endif
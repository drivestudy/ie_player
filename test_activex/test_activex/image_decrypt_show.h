
// filePath	string	经过加密的图像路径
// authorizationInfo	map<string,string>	权利描述信息
// isShow	bool	表明是否显示


#ifndef  __IMAGE_DECRYPT_SHOW_H__
#define  __IMAGE_DECRYPT_SHOW_H__


#include "windows.h"
#include "atlimage.h"
#include "iostream"
#include "string"
#include "fstream"
#include "map"

using namespace std;
using namespace ATL;

#include "IImageDecryptShow.h"


class ImageDecryptShow : public IImageDecryptShow
{
public:
	ImageDecryptShow();
	~ImageDecryptShow();

	inline CImage& GetImage()
	{
		return image_ ;
	}

	inline void SetIsAlreadyDownLoad(bool is_already_down_load)
	{
		is_already_down_load_ = is_already_down_load;
	}

	inline bool GetIsAlreadyDownLoad()
	{
		return is_already_down_load_;
	}

	void ParaseImageAndShow(std::string file_path, std::string decrypt_key);

private:
	// 保存图像
	CImage image_;
	// 从加密的图像文件中，解密出来的数据
	char *out_data_;
	int  out_data_length_;

	// 表明是否已经把数据进行缓存
	bool is_already_down_load_;

	void LoadMemImage(void *out_data, long out_data_length);
};

#endif
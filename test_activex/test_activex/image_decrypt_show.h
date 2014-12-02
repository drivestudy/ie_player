
// filePath	string	�������ܵ�ͼ��·��
// authorizationInfo	map<string,string>	Ȩ��������Ϣ
// isShow	bool	�����Ƿ���ʾ


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
	// ����ͼ��
	CImage image_;
	// �Ӽ��ܵ�ͼ���ļ��У����ܳ���������
	char *out_data_;
	int  out_data_length_;

	// �����Ƿ��Ѿ������ݽ��л���
	bool is_already_down_load_;

	void LoadMemImage(void *out_data, long out_data_length);
};

#endif
#include "image_decrypt_show.h"
#include "decrypt_relation.h"
#include "get_web_data_from_url.h"

#define WEB_FILE_CACHE		"plugins_cache"
#define BUFF_LENG			256
#define DECRYPT_PART_LENG	16


#ifdef USE_URLDOWNLOADTOFILE
// 初始化远程文件名
const char *GetWebDataFromUrl::local_file_name = "";
#endif // USE_URLDOWNLOADTOFILE

ImageDecryptShow ::ImageDecryptShow()
	:out_data_(NULL)
	,out_data_length_(0)
	,is_already_down_load_(false)
{
}


ImageDecryptShow ::~ImageDecryptShow()
{
	RELESE_ARRAY(out_data_);
}


// 要改这里的GlobalAlloc这个好像已经过时
void ImageDecryptShow::LoadMemImage(void *out_data, long out_data_length)
{
	HGLOBAL hGlobal  =  GlobalAlloc(GMEM_MOVEABLE, out_data_length);
	void* pData  =  GlobalLock(hGlobal);
	::memcpy(pData, out_data, out_data_length);
	GlobalUnlock(hGlobal);
	IStream* pStream  =  NULL;
	if  (CreateStreamOnHGlobal(hGlobal, TRUE,  & pStream)  ==  S_OK)
	{
		if (SUCCEEDED(image_.Load(pStream)))
		{
			// 成功读取之后，什么都不做？
			printf("zeng");
		}
		pStream->Release();    
	}
	GlobalFree(hGlobal);
}


void ImageDecryptShow::ParaseImageAndShow(std::string file_path, std::string decrypt_key)
{
	// 先判断一下是本地文件还是网络文件
	bool is_web_file_path = false;
	int file_length_encrypt = 0;

	string::size_type substr_pos = file_path.find("http://");
	if (substr_pos != string::npos)
	{
		is_web_file_path = true;
	}

	if (out_data_)
	{
		delete[] out_data_;
		out_data_  = NULL;
	}

#ifdef USE_URLDOWNLOADTOFILE
	if (is_web_file_path)
	{
		// 先从web中把文件获取到本地中
		if (!GetWebDataFromUrl::SaveWebFileToLocal(file_path.c_str(), WEB_FILE_CACHE))
		{
			MessageBox(0, "web file visit fail..", "error", MB_OK);
			return;
		}
		file_path = GetWebDataFromUrl::GetLocalFileName();
	}

	// 从本地文件中取出数据的
	ifstream fin_encrypt(file_path, ios::binary);
	if (!fin_encrypt)
	{
		return;
	}
	fin_encrypt.seekg(0, ios::end);
	file_length_encrypt = fin_encrypt.tellg();

	out_data_ = new char[file_length_encrypt + 1];
	::memset(out_data_, 0, file_length_encrypt + 1);
	

	// 从文件中解密到数组中
	if (decrypt_key.length() > 0)
	{		
		DecryptRelation::AesDecryptFromFileToBytes(file_path, (unsigned char *)out_data_,
			out_data_length_, decrypt_key.c_str(), 16);
	}
	else
	{
		fin_encrypt.seekg(0, ios::beg);
		fin_encrypt.read(out_data_, file_length_encrypt);
	}

	fin_encrypt.close();

#endif // USE_URLDOWNLOADTOFILE

#ifdef USE_LIBCURL
	// 从本地文件中取出数据的
	if (!is_web_file_path)
	{
		// 需要计算出out_data的大小
		ifstream fin_encrypt(file_path, ios::binary);
		if (!fin_encrypt)
		{
			return;
		}

		fin_encrypt.seekg(0, ios::end);
		file_length_encrypt = fin_encrypt.tellg();

		out_data_ = new char[file_length_encrypt + 1];
		memset(out_data_, 0, file_length_encrypt + 1);
		fin_encrypt.close();

		// 从文件中解密到数组中
		DecryptRelation::AesDecryptFromFileToBytes(file_path, (unsigned char *)out_data_,
			out_data_length_, decrypt_key.c_str(), 16);
	}
	else // 从web路径中取出文件
	{
		char *original_web_data;
		original_web_data = GetWebDataFromUrl::GetPageContent(file_path);

		out_data_ = new char[GetWebDataFromUrl::GetWebDataLength()];
		memset(out_data_, 0, GetWebDataFromUrl::GetWebDataLength());

		char *in_data = new char[16];
		char *in_data_tmp  = new char[16];

		// 把得到的数据，解密出来放到out_data_中
		int index = 0;
		while (1)
		{
			// 如果索引超过了大小 那么直接break
			if (index + 16 > GetWebDataFromUrl::GetWebDataLength())
			{
				// 直接写入最后的字节
				int i = GetWebDataFromUrl::GetWebDataLength() - index;
				memcpy(out_data_ + index, original_web_data + index, i);
				break;
			}

			// 取出original_web_data+index的数据填入到in_data中
			memset(in_data, 0, 16);
			memset(in_data_tmp, 0, 16);
			memcpy(in_data, original_web_data + index, 16);

			// 对每一段数据进行解密操作
			DecryptRelation::AesDecryptBytes((const unsigned char *)in_data, 
				(unsigned char *)in_data_tmp, "0123456789abcdef0123456789abcdee");

			// 把in_data_tmp中的数据填入到out_data_中
			memcpy(out_data_ + index, in_data_tmp, 16);

			index += 16;
		}

		file_length_encrypt = GetWebDataFromUrl::GetWebDataLength();

		if (in_data)
		{
			delete[] in_data;
			in_data = NULL;
		}
		if (in_data_tmp)
		{
			delete[] in_data_tmp;
			in_data_tmp = NULL;
		}
	}

#endif // USE_LIBCURL

	// 显示出图片
	LoadMemImage(out_data_, file_length_encrypt);
}


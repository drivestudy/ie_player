#include "fstream"
#include "get_web_data_from_url.h"


#ifdef WIN32

#ifdef USE_URLDOWNLOADTOFILE

// use URLDownloadToFile =================================== is win32 platform
// save the web file to local
/************************************************************************/
/* 函数名：GetTheFileName
/* 功	 能:  从文件路径中得到文件名
/* 返回值：文件名
/************************************************************************/
const char *GetTheFileName(const char *file_path)
{
	char c = '/';
	const char *file_name_tmp;
	const char *file_name = file_path;

	// 找到最后一个/ 就能得到文件名
	// 首先判断第一位是不是/，如果不是再进去while
	if (file_path[0] == '/' && file_path[1] != '/')
	{
//		cout << file_name + 1 << endl;
		return file_name  + 1;
	}
	else
	{
		while (1)
		{
			file_name_tmp = strchr(file_name + 1, c);
			if (file_name_tmp == NULL)
			{
				break;
			}
			file_name = file_name_tmp + 1;
		}
	}

	return file_name;
}


/************************************************************************/
/* 函数名：SaveWebFileToLocal
/* 功	 能: 从url_file_path下载文件到url_file_cache_directory中
/* 返回值：成功，返回true；失败，返回false
/************************************************************************/
bool GetWebDataFromUrl::SaveWebFileToLocal(const char *url_file_path, char *url_file_cache_directory)
{
	HRESULT hr;
	Callback callback;

	local_file_name  = GetTheFileName(url_file_path);

	// 判断是否存在文件目录url_file_cache_directory，如果不存在则创建出来，如果存在则把那个文件目录设置成当前的工作目录
	if (url_file_cache_directory[0] != ' ')
	{
		if (strlen(url_file_cache_directory) > 0)
		{
			if (_access(url_file_cache_directory, 0) != -1)
			{
				cout << "已经存在" << endl;
			}
			else
			{
				if(!CreateDirectory(url_file_cache_directory,NULL)) // //绝对路径创建目录
				{
					printf("创建%s目录错误\n",url_file_cache_directory);
					// 这里有error
				}
			}
			SetCurrentDirectory(url_file_cache_directory);  //设置
		}
	}

	// 如果不清空缓存，那么第二次还下载此文件，会得到文件信息为0
	BOOL delete_url_cache_flag = DeleteUrlCacheEntry(url_file_path);
	if (!delete_url_cache_flag)
	{
		puts("DeleteUrlCacheEntry failed..");
	}

	hr = URLDownloadToFile (NULL,		// ptr to ActiveX container
		(LPCSTR)url_file_path,			// URL to get
		(LPCSTR)local_file_name,		// file to store data in
		0,								// reserved
		&callback						// ptr to IBindStatusCallback
		);

	// 对文件进行判断
	std::ifstream fin;
	fin.open(local_file_name);
	if (fin)
	{
#ifdef DEBUG
		cout << "file down load complete" << endl;
#endif
		return true;
	}

	return false;
}

#endif // USE_LIBCURL

#endif // WIN32


// use libcurl ============================================= not win32 platform
#ifdef USE_LIBCURL
//static member variable define
CURL* GetWebDataFromUrl::curl_		= NULL;
char* GetWebDataFromUrl::web_data_  = NULL; //当前保存的网页源码
int GetWebDataFromUrl::data_length  = 0;


GetWebDataFromUrl::GetWebDataFromUrl(void)
{
	web_data_ = NULL;
}


GetWebDataFromUrl::~GetWebDataFromUrl(void)
{

}


/************************************************************************/
/* 函数名：Initialize
/* 功	 能: 初始化libcurl库
/* 返回值：成功，返回true；失败，返回false
/************************************************************************/
bool GetWebDataFromUrl::Initialize()
{
	curl_global_init(CURL_GLOBAL_ALL);  
	curl_ = curl_easy_init();
	if (curl_)
	{
		curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteFunc);  
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &web_data_);
		curl_easy_setopt(curl_, CURLOPT_ACCEPT_ENCODING, "gzip");
		curl_easy_setopt(curl_, CURLOPT_NOSIGNAL, 1);
	}
	else
	{
		MessageBoxA(NULL,"GetPageByURL::Initialize Failed!", "GetPageByURL::Initialize", MB_ICONERROR);
		return false;
	}
	return true;
}


/************************************************************************/
/* 函数名：WriteFunc
/* 功  能: libcurl会调用这个标准的函数，
/*	size_t function( void *ptr, size_t size, size_t nmemb, void *userdata);
/*	以提供格式化网页数据的机会
/*	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc); 
/* 返回值：返回buffer的大小
/************************************************************************/
size_t GetWebDataFromUrl::WriteFunc(char *data, size_t size, size_t nmemb)
{
	size_t len = size * nmemb;

	if (web_data_ == NULL)
	{
		web_data_ = new char[len];
		memcpy(web_data_, data, len);
		data_length += len;
	}
	else
	{
		// 追加在末尾
		char *web_data_tmp = new char[data_length];
		memcpy(web_data_tmp, web_data_, data_length);
		delete[] web_data_;
		web_data_ = new char[data_length + len];
		memcpy(web_data_, web_data_tmp, data_length);
		memcpy(web_data_ + data_length, data, len);
		data_length += len;

		delete[] web_data_tmp;
		web_data_tmp = NULL;
	}

	return len;
}


/************************************************************************/
/* 函数名：GetPage
/* 功  能: 根据url，抓取对应的网页源码；使用了libcurl库
/* 返回值：成功，返回包含网页源码的字符串；失败，返回空字符串
/************************************************************************/
char* GetWebDataFromUrl::GetPageContent(const string& url_str)
{
	_ASSERT("" != url_str);
	if(!curl_)
	{
		return false;
	}


	if (web_data_)
	{
		delete[] web_data_;
		web_data_ = NULL;
	}

	curl_easy_setopt(curl_, CURLOPT_URL, url_str.c_str());
	CURLcode res = curl_easy_perform(curl_);
	if(res != CURLE_OK)
	{
		MessageBoxA(NULL,"curl call false!", "GetWebDataFromUrl::GetPage2", MB_ICONERROR);
		return false;
	}
	else
	{
		return web_data_;
	}
}


/************************************************************************/
/* 函数名：GetPage
/* 功  能: 根据url，抓取对应的网页源码；使用了libcurl库
/* 返回值：成功，返回包含网页源码的字符串；失败，返回空字符串
/************************************************************************/
bool GetWebDataFromUrl::GetPageContent2(const string& url_str, //url字符串
										string& page_content    //输出参数，返回对应页面源码
										)
{
	_ASSERT("" != url_str);
	if(!curl_)
	{
		MessageBoxA(NULL,"curl is null!", "GetWebDataFromUrl::GetPage", MB_ICONERROR);
		return false;
	}

	if (web_data_)
	{
		delete[] web_data_;
		web_data_ = NULL;
	}

	curl_easy_setopt(curl_, CURLOPT_URL, url_str.c_str());
	CURLcode res = curl_easy_perform(curl_);
	if(res != CURLE_OK)
	{
		MessageBoxA(NULL,"curl call false!", "GetWebDataFromUrl::GetPage", MB_ICONERROR);
		return false;
	}
	else
	{
		page_content.append(web_data_, data_length);
		return true;
	}
}


/************************************************************************/
/* 函数名：Cleanup
/* 功  能: 清理内存
/* 返回值：无
/************************************************************************/
void GetWebDataFromUrl::Cleanup()
{
	if(curl_)
	{
		/* always cleanup */
		curl_easy_cleanup(curl_);
		curl_ = NULL;
	}

	if (web_data_)
	{
		delete[] web_data_;
		web_data_ = NULL;
	}
}

#endif // USE_LIBCURL
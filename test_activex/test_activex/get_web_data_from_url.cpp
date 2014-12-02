#include "fstream"
#include "get_web_data_from_url.h"


#ifdef WIN32

#ifdef USE_URLDOWNLOADTOFILE

// use URLDownloadToFile =================================== is win32 platform
// save the web file to local
/************************************************************************/
/* ��������GetTheFileName
/* ��	 ��:  ���ļ�·���еõ��ļ���
/* ����ֵ���ļ���
/************************************************************************/
const char *GetTheFileName(const char *file_path)
{
	char c = '/';
	const char *file_name_tmp;
	const char *file_name = file_path;

	// �ҵ����һ��/ ���ܵõ��ļ���
	// �����жϵ�һλ�ǲ���/����������ٽ�ȥwhile
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
/* ��������SaveWebFileToLocal
/* ��	 ��: ��url_file_path�����ļ���url_file_cache_directory��
/* ����ֵ���ɹ�������true��ʧ�ܣ�����false
/************************************************************************/
bool GetWebDataFromUrl::SaveWebFileToLocal(const char *url_file_path, char *url_file_cache_directory)
{
	HRESULT hr;
	Callback callback;

	local_file_name  = GetTheFileName(url_file_path);

	// �ж��Ƿ�����ļ�Ŀ¼url_file_cache_directory������������򴴽������������������Ǹ��ļ�Ŀ¼���óɵ�ǰ�Ĺ���Ŀ¼
	if (url_file_cache_directory[0] != ' ')
	{
		if (strlen(url_file_cache_directory) > 0)
		{
			if (_access(url_file_cache_directory, 0) != -1)
			{
				cout << "�Ѿ�����" << endl;
			}
			else
			{
				if(!CreateDirectory(url_file_cache_directory,NULL)) // //����·������Ŀ¼
				{
					printf("����%sĿ¼����\n",url_file_cache_directory);
					// ������error
				}
			}
			SetCurrentDirectory(url_file_cache_directory);  //����
		}
	}

	// �������ջ��棬��ô�ڶ��λ����ش��ļ�����õ��ļ���ϢΪ0
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

	// ���ļ������ж�
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
char* GetWebDataFromUrl::web_data_  = NULL; //��ǰ�������ҳԴ��
int GetWebDataFromUrl::data_length  = 0;


GetWebDataFromUrl::GetWebDataFromUrl(void)
{
	web_data_ = NULL;
}


GetWebDataFromUrl::~GetWebDataFromUrl(void)
{

}


/************************************************************************/
/* ��������Initialize
/* ��	 ��: ��ʼ��libcurl��
/* ����ֵ���ɹ�������true��ʧ�ܣ�����false
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
/* ��������WriteFunc
/* ��  ��: libcurl����������׼�ĺ�����
/*	size_t function( void *ptr, size_t size, size_t nmemb, void *userdata);
/*	���ṩ��ʽ����ҳ���ݵĻ���
/*	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc); 
/* ����ֵ������buffer�Ĵ�С
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
		// ׷����ĩβ
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
/* ��������GetPage
/* ��  ��: ����url��ץȡ��Ӧ����ҳԴ�룻ʹ����libcurl��
/* ����ֵ���ɹ������ذ�����ҳԴ����ַ�����ʧ�ܣ����ؿ��ַ���
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
/* ��������GetPage
/* ��  ��: ����url��ץȡ��Ӧ����ҳԴ�룻ʹ����libcurl��
/* ����ֵ���ɹ������ذ�����ҳԴ����ַ�����ʧ�ܣ����ؿ��ַ���
/************************************************************************/
bool GetWebDataFromUrl::GetPageContent2(const string& url_str, //url�ַ���
										string& page_content    //������������ض�Ӧҳ��Դ��
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
/* ��������Cleanup
/* ��  ��: �����ڴ�
/* ����ֵ����
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
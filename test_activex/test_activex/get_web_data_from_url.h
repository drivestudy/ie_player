#ifndef __GET_WEB_DATA_FROM_URL_H__
#define __GET_WEB_DATA_FROM_URL_H__

#define USE_URLDOWNLOADTOFILE

// 如果使用libcurl需要忽略掉libcmtd.lib
#ifdef USE_LIBCURL
#ifdef DEBUG
#pragma comment(linker,"/nodefaultlib:libcmtd.lib")
#else
#pragma comment(linker,"/nodefaultlib:libcmt.lib")
#endif // DEBUG
#endif // USE_LIBCURL


#include "iostream"
using namespace std;


// use URLDownloadToFile =================================== is  win32 platform
#ifdef WIN32

#ifdef USE_URLDOWNLOADTOFILE

#include "windows.h"
#include "io.h"

#include "URLmon.h"
#pragma comment(lib, "URLmon.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "wininet.h"
#pragma comment(lib, "wininet.lib")


class Callback : public IBindStatusCallback  
{
public:
	Callback()  {}
	~Callback() {}

	// IBindStatusCallback methods.  Note that the only method called by IE
	// is OnProgress(), so the others just return E_NOTIMPL.

	STDMETHOD(OnStartBinding)(
		/* [in] */ DWORD dwReserved,
		/* [in] */ IBinding __RPC_FAR *pib)
	{ return E_NOTIMPL; }

	STDMETHOD(GetPriority)(
		/* [out] */ LONG __RPC_FAR *pnPriority)
	{ return E_NOTIMPL; }

	STDMETHOD(OnLowResource)(
		/* [in] */ DWORD reserved)
	{ return E_NOTIMPL; }

	STDMETHOD(OnProgress)(
		/* [in] */ ULONG ulProgress,
		/* [in] */ ULONG ulProgressMax,
		/* [in] */ ULONG ulStatusCode,
		/* [in] */ LPCWSTR wszStatusText)
	{ 
		//	return E_NOTIMPL;
		static char current_progress_msg [256];
		static char already_download_size [256], file_total_size [256];
		if (ulProgressMax)
		{
			StrFormatByteSize ( ulProgress, already_download_size, 256 );
			StrFormatByteSize ( ulProgressMax, file_total_size, 256 );

			if ( 0 != ulProgressMax )
			{
				sprintf ( current_progress_msg, "Downloaded %s of %s",
					already_download_size, file_total_size );
			}
			else
			{
				sprintf ( current_progress_msg, "Downloaded %s (total size unknown)",
					already_download_size );
			}

			puts(current_progress_msg);
		}

		return S_OK;
	}

	STDMETHOD(OnStopBinding)(
		/* [in] */ HRESULT hresult,
		/* [unique][in] */ LPCWSTR szError)
	{ return E_NOTIMPL; }

	STDMETHOD(GetBindInfo)(
		/* [out] */ DWORD __RPC_FAR *grfBINDF,
		/* [unique][out][in] */ BINDINFO __RPC_FAR *pbindinfo)
	{ return E_NOTIMPL; }

	STDMETHOD(OnDataAvailable)(
		/* [in] */ DWORD grfBSCF,
		/* [in] */ DWORD dwSize,
		/* [in] */ FORMATETC __RPC_FAR *pformatetc,
		/* [in] */ STGMEDIUM __RPC_FAR *pstgmed)
	{ return E_NOTIMPL; }

	STDMETHOD(OnObjectAvailable)(
		/* [in] */ REFIID riid,
		/* [iid_is][in] */ IUnknown __RPC_FAR *punk)
	{ return E_NOTIMPL; }

	// IUnknown methods.  Note that IE never calls any of these methods, since
	// the caller owns the IBindStatusCallback interface, so the methods all
	// return zero/E_NOTIMPL.

	STDMETHOD_(ULONG,AddRef)()
	{ return 0; }

	STDMETHOD_(ULONG,Release)()
	{ return 0; }

	STDMETHOD(QueryInterface)(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{ return E_NOTIMPL; }
};



class GetWebDataFromUrl
{
public:
	// save the web file to local
	static bool SaveWebFileToLocal(const char *url_file_path, char *url_file_cache_directory);

	static inline const char *GetLocalFileName()
	{
		return local_file_name;
	}

private:
	GetWebDataFromUrl();
	static const char *local_file_name;
};

#endif // USE_LIBCURL

#endif // WIN32


// use libcurl ============================================= not win32 platform
#ifdef USE_LIBCURL

#include "curl.h"
#include "string"


class GetWebDataFromUrl
{
public:
	GetWebDataFromUrl(void);
public:
	~GetWebDataFromUrl(void);
private:
	static CURL* curl_;
	static char* web_data_;
	static int data_length;
private:
	static size_t WriteFunc(char *data, size_t size, size_t nmemb/*, string *writerData*/);
public:
	static bool Initialize();
	static char* GetPageContent(const string& url_str);
	static bool GetPageContent2(const string& url_str, string& page_content);
	static void Cleanup();

	// 返回从web页面获取的数据的长度
	static inline int GetWebDataLength()
	{
		return data_length;
	}
};

#endif // USE_LIBCURL


#endif // __GET_WEB_DATA_FROM_URL_H__

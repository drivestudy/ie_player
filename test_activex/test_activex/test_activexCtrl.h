#pragma once

#include "iostream"
#include "string"

#include <ObjSafe.h>

#include "atlimage.h"
using namespace ATL;

#include "image_decrypt_show.h"

// test_activexCtrl.h : Ctest_activexCtrl ActiveX 控件类的声明。


// Ctest_activexCtrl : 有关实现的信息，请参阅 test_activexCtrl.cpp。
class CDuiFrameWnd;
class Ctest_activexCtrl : public COleControl
{
	DECLARE_DYNCREATE(Ctest_activexCtrl)

// 构造函数
public:
	Ctest_activexCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~Ctest_activexCtrl();

	DECLARE_OLECREATE_EX(Ctest_activexCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(Ctest_activexCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(Ctest_activexCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(Ctest_activexCtrl)	   // 类型名称和杂项状态

	DECLARE_INTERFACE_MAP()
	BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)   
		STDMETHOD_(HRESULT,   GetInterfaceSafetyOptions)   (     
		/*   [in]   */   REFIID   riid,   
		/*   [out]   */   DWORD   __RPC_FAR   *pdwSupportedOptions,   
		/*   [out]   */   DWORD   __RPC_FAR   *pdwEnabledOptions   
		);  

		STDMETHOD_(HRESULT,   SetInterfaceSafetyOptions)   (     
			/*   [in]   */   REFIID   riid,   
			/*   [in]   */   DWORD   dwOptionSetMask,   
			/*   [in]   */   DWORD   dwEnabledOptions   
			);  

	END_INTERFACE_PART(ObjSafe);

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
	};

private:
	std::string file_path_;		// 图片资源的路径
	std::string mplayer_path_;	// 播放器的路径
	std::string pic_decrypt_key_;//解密图像的key
	CImage image_;				// 所展示图片的内存
	
	IImageDecryptShow			*image_decrypt_show_;

	bool is_show_player_;		// 播放器是否已经初始化过了
	bool is_can_play_type_;		// 是播放器可以支持的视频媒体类型
	bool is_can_show_type_;		// 是可支持的图片文件类型

	bool is_chose_window_size_html_sign_; // 对应页面html标志----是否自动调整播放宽和高

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose(DWORD dwSaveOption);

private:
	void FileTypeJudge(std::string file_path);
};


// test_activexCtrl.cpp : Ctest_activexCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "test_activex.h"
#include "test_activexCtrl.h"
#include "test_activexPropPage.h"
#include "afxdialogex.h"
#include "iostream"
#include "fstream"
#include "map"

#include "decrypt_relation.h"

#include "DuiFrameWnd.h"


#define RELEASE(p) if (p)	\
{							\
	delete p;				\
	p = NULL;				\
}


#define PLAYER_PATH							"c:\\plugins_need\\ffplay.exe"
#define DUILIB_SET_RESOURCE_PATH			"c:\\plugins_need\\player_skin\\"
#define DUILIB_SET_RESOURCE_ZIP_PRE_PATH	"c:\\plugins_need\\"
#define DUILIBE_SET_RESOURCE_ZIP_NAME		"resource.zip"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool is_chose_window_size = false; // 触发窗体的OnSize事件
CDuiFrameWnd *g_dui_frame_wnd = NULL;

IMPLEMENT_DYNCREATE(Ctest_activexCtrl, COleControl)

// 消息映射

BEGIN_MESSAGE_MAP(Ctest_activexCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// 调度映射

BEGIN_DISPATCH_MAP(Ctest_activexCtrl, COleControl)
	DISP_FUNCTION_ID(Ctest_activexCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// 事件映射

BEGIN_EVENT_MAP(Ctest_activexCtrl, COleControl)
END_EVENT_MAP()


//接口映射
BEGIN_INTERFACE_MAP(Ctest_activexCtrl, COleControl)   
	INTERFACE_PART(Ctest_activexCtrl, IID_IObjectSafety, ObjSafe)   
END_INTERFACE_MAP() 


// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(Ctest_activexCtrl, 1)
	PROPPAGEID(Ctest_activexPropPage::guid)
END_PROPPAGEIDS(Ctest_activexCtrl)

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(Ctest_activexCtrl, "TEST_ACTIVEX.test_activexCtrl.1",
	0x458fe3ff, 0x3843, 0x49c6, 0x80, 0x2d, 0xac, 0x10, 0xd9, 0x76, 0x96, 0x9)

	// 键入库 ID 和版本

	IMPLEMENT_OLETYPELIB(Ctest_activexCtrl, _tlid, _wVerMajor, _wVerMinor)

	// 接口 ID

const IID IID_Dtest_activex       = { 0x94A191F0, 0xD0D6, 0x46ED, { 0xA8, 0x2D, 0x7B, 0xAA, 0x86, 0x28, 0x7, 0x31 } };
const IID IID_Dtest_activexEvents = { 0x3E382D9E, 0x3FFF, 0x433D, { 0x87, 0x17, 0x5A, 0x60, 0xF8, 0xC7, 0xBD, 0x70 } };

// 控件类型信息

static const DWORD _dwtest_activexOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(Ctest_activexCtrl, IDS_TEST_ACTIVEX, _dwtest_activexOleMisc)

// Ctest_activexCtrl::Ctest_activexCtrlFactory::UpdateRegistry -
// 添加或移除 Ctest_activexCtrl 的系统注册表项

BOOL Ctest_activexCtrl::Ctest_activexCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。
	if (bRegister)
		return AfxOleRegisterControlClass(
		AfxGetInstanceHandle(),
		m_clsid,
		m_lpszProgID,
		IDS_TEST_ACTIVEX,
		IDB_TEST_ACTIVEX,
		afxRegApartmentThreading,
		_dwtest_activexOleMisc,
		_tlid,
		_wVerMajor,
		_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// Ctest_activexCtrl::Ctest_activexCtrl - 构造函数

Ctest_activexCtrl::Ctest_activexCtrl()
{
	InitializeIIDs(&IID_Dtest_activex, &IID_Dtest_activexEvents);
	// TODO: 在此初始化控件的实例数据
	image_decrypt_show_ = NULL;

	is_show_player_   = false;
	is_can_play_type_ = false;
	is_can_show_type_ = false;

	file_path_		  = "";
	mplayer_path_	  = "";
	pic_decrypt_key_  = "";

	is_chose_window_size_html_sign_ = false;
}


// Ctest_activexCtrl::~Ctest_activexCtrl - 析构函数

Ctest_activexCtrl::~Ctest_activexCtrl()
{
	// TODO: 在此清理控件的实例数据。
	RELEASE(image_decrypt_show_);
	RELEASE(g_dui_frame_wnd);
}


// Ctest_activexCtrl::OnDraw - 绘图函数

void Ctest_activexCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;
	
	// TODO: 用您自己的绘图代码替换下面的代码。
	if (is_can_show_type_)
	{
		HDC hdc;
		hdc = pdc->m_hDC;
		if (!image_decrypt_show_->GetImage().IsNull())
		{
			int width  = image_decrypt_show_->GetImage().GetWidth();
			int height = image_decrypt_show_->GetImage().GetHeight();
			RECT imgRc = { 0, 0, image_decrypt_show_->GetImage().GetWidth(), image_decrypt_show_->GetImage().GetHeight() };

			image_decrypt_show_->GetImage().Draw(hdc, rcBounds, imgRc);
		}
		else
		{
			if (!image_decrypt_show_->GetIsAlreadyDownLoad())
			{
				// 只解析一次，不重复解析，如果需要重复解析，需要自己设置
				image_decrypt_show_->ParaseImageAndShow(file_path_, pic_decrypt_key_);
				Invalidate();
			}
		}
	}
	else
	{
		CRect rect;
		CBrush backBrush;
		backBrush.CreateSolidBrush(RGB(255, 255, 255));
		this->GetClientRect(&rect);
		pdc->FillRect(&rect, &backBrush); //用FillRect成员函数利用笔刷填充指定区域
		backBrush.DeleteObject();
	}
}


// Ctest_activexCtrl::DoPropExchange - 持久性支持med

static char const* video_file_gather[] = 
{
	".mp3", 
	".mp4",
	".avi",
	".mov",
	".mpeg",
	".flv",
	".mkv",
	"null"
};


static char const* pic_file_gather[] = 
{
	".jpg", 
	".jpeg",
	".bmp",
	".gif",
	".png",
	"null"
};


void Ctest_activexCtrl::FileTypeJudge(std::string file_path)
{
	int which = file_path.find_last_of(".");

	if (!file_path.compare(""))
	{
		return;
	}

	std::string extend_name = file_path.substr(which, file_path.length());
	
	int index = 0;
	// 判断是否是可支持的视频文件
	while (true)
	{
		if (!strcmp(video_file_gather[index], "null"))
		{
			break;
		}
		else
		{
			if (!strcmp(extend_name.c_str(), video_file_gather[index]))
			{
				is_can_play_type_ = !is_can_play_type_;
			//	AfxMessageBox("is_can_play_type_");
				return;
			}
		}
		index++;
	}

	// 判断是否是可支持的图片文件类型
	index = 0;
	while (true)
	{
		if (!strcmp(pic_file_gather[index], "null"))
		{
			break;
		}
		else
		{
			if (!strcmp(extend_name.c_str(), pic_file_gather[index]))
			{
				is_can_show_type_ = !is_can_show_type_;
			//	AfxMessageBox("is_can_show_type_");
				return;
			}
		}
		index++;
	}

	AfxMessageBox("this file type nonsupport.");
}


VOID CALLBACK TimeProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTimer)
{
	if (g_dui_frame_wnd->GetIsPlayStatus())
	{
		// 这里调用增加进度条数值
		g_dui_frame_wnd->OnPosChanged(hWnd, 0, 0, enum_ForWardOrBackWard::EM_NULL);
	}
}


void Ctest_activexCtrl::OnClose(DWORD dwSaveOption)
{
	// TODO: 响应页面关闭的事件，假如析构函数不执行(OnClose比析构执行时间更快，以防析构函数没有被执行到的情况)。
	if (image_decrypt_show_)
	{
		delete image_decrypt_show_;
		image_decrypt_show_ = NULL;
	}

	if (g_dui_frame_wnd)
	{
		g_dui_frame_wnd->ResetAVPlayer();
	}

	COleControl::OnClose(dwSaveOption);
}


void Ctest_activexCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
	// 如果当前的file_path_不为空串，那么直接return
	if (file_path_.compare(""))
	{
		return;
	}

	CString file_path;
	CString mplayer_path;
	CString is_change;
	CString pic_decrypt_key;

	PX_String(pPX, _T("FilePath"), file_path, _T(""));
	PX_String(pPX, _T("PlayerPath"), mplayer_path, _T(""));
	PX_String(pPX, _T("pic_decrypt_key"), pic_decrypt_key, _T(""));
	PX_String(pPX, _T("changesize"), is_change, _T(""));
	// 判断html是否写了根据视频的信息来设定宽和高
	if (!is_change.CompareNoCase("1") || !is_change.CompareNoCase("true"))
	{
		is_chose_window_size_html_sign_ = true;
	}

	file_path_ = file_path.GetBuffer();
	pic_decrypt_key_ = pic_decrypt_key.GetBuffer();

	// 判断是媒体资源还是图片资源
	FileTypeJudge(file_path_);

	mplayer_path_ = mplayer_path.GetBuffer();

	// 应该首先判断资源类型
	if (mplayer_path_.empty())
	{
		mplayer_path_ = PLAYER_PATH;
	}
	if (file_path_.empty())
	{
	//	MessageBox("file path is null");
	}

	if (is_can_play_type_)
	{
		// 这是处理页面的刷新事件的
		if (g_dui_frame_wnd)
		{
			delete g_dui_frame_wnd;
			g_dui_frame_wnd = NULL;
		}
		g_dui_frame_wnd = new CDuiFrameWnd(_T("XMP.xml"));

		// 这样才能让触发duilib高和宽的更改
		is_chose_window_size = false;
	}
	else if (is_can_show_type_)
	{
		image_decrypt_show_ = new ImageDecryptShow();
	}
}


// Ctest_activexCtrl::OnResetState - 将控件重置为默认状态

void Ctest_activexCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}


// Ctest_activexCtrl::AboutBox - 向用户显示“关于”框

void Ctest_activexCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_TEST_ACTIVEX);
	dlgAbout.DoModal();
}


// Ctest_activexCtrl 消息处理程序

int Ctest_activexCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!is_show_player_)
	{
		if (is_can_play_type_)
		{
			RECT rect;
			::GetClientRect(GetSafeHwnd(), &rect);

			int width  = rect.right  - rect.left;
			int height = rect.bottom - rect.top;

			HWND hWnd = GetSafeHwnd();
			CPaintManagerUI::SetInstance(GetModuleHandle(0));

			////////////////////////////使用文件资源//////////////////////////////////////////////
//			CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("player_skin"));
//			CPaintManagerUI::SetResourcePath(DUILIB_SET_RESOURCE_PATH);
			//////////////////////////////////////////////////////////////////////////
			
			////////////////////////////使用zip资源//////////////////////////////////////////////
			CPaintManagerUI::SetResourcePath(DUILIB_SET_RESOURCE_ZIP_PRE_PATH);
			CPaintManagerUI::SetResourceZip(DUILIBE_SET_RESOURCE_ZIP_NAME);
			//////////////////////////////////////////////////////////////////////////
			
			g_dui_frame_wnd->Create(GetSafeHwnd(), _T("DUIWnd"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES, 0, 0, 0, 0);
			g_dui_frame_wnd->ShowWindow(1);
			g_dui_frame_wnd->SetParentHwnd(GetSafeHwnd());
			g_dui_frame_wnd->SetFilePath(file_path_);

			// 如果对视频播窗口大小进行限定，那么使用下边的语句 暂时设定宽和高都是400
			if (!is_chose_window_size_html_sign_) // 对窗口进行高宽的调整
			{
				g_dui_frame_wnd->SetPlayerWindowsWidth(width);
				g_dui_frame_wnd->SetPlayerWindowsHeight(height - 115); // 需要减去控件的高度

				is_chose_window_size_html_sign_ = false;
			}

			// 在这里post一下WM_SIZE看看能不能设定窗口的初始大小
			::PostMessage(hWnd, WM_SIZE, 0, 0);
			int timer_sign = 1;
			::SetTimer(0, timer_sign, 1000, TimeProc);

			is_show_player_ = !is_show_player_;
		}
	}

	return 0;
}


void Ctest_activexCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码
	if (is_can_play_type_)
	{
		HWND dui_frame_hwnd;
		int adjust_height;
		dui_frame_hwnd = g_dui_frame_wnd->GetHWND();
		if (!is_chose_window_size)
		{
			::MoveWindow(dui_frame_hwnd, 0, 0, cx, cy, true);

			is_chose_window_size = !is_chose_window_size;
		}

		// TODO：需要在这里进行播放窗口的调整
		// 首先要获得视频的高和宽，然后再去更新，这就是一个标志
		if (g_dui_frame_wnd->GetIsAlreadyObtainVideoInfo())
		{

			if (!g_dui_frame_wnd->GetIsAlreadyChangeWindows())
			{
				// TODO：通过设定的窗口大小，改变播放器的窗口大小
				if (g_dui_frame_wnd->GetPlayerWindowsWidth() != 0)

				{
					adjust_height = g_dui_frame_wnd->GetPlayerWindowsHeight() + 115;
					::MoveWindow(dui_frame_hwnd, 0, 0, g_dui_frame_wnd->GetPlayerWindowsWidth(), adjust_height, true);
				}
				else
				{
					// 通过视频宽和高，改变播放器的窗口大小
					adjust_height = g_dui_frame_wnd->GetVideoHeight() + 115; // 高度因为控件需要调整
					::MoveWindow(dui_frame_hwnd, 0, 0, g_dui_frame_wnd->GetVideoWidth(), adjust_height, true);
				}

				g_dui_frame_wnd->SetIsAlreadyChangeWindows(true);
			}
		}
		UpdateWindow();
	}
}


// IObjectSafety   member   functions
// Delegate   AddRef,   Release,   QueryInterface
ULONG FAR EXPORT Ctest_activexCtrl::XObjSafe::AddRef()
{
	METHOD_PROLOGUE(Ctest_activexCtrl, ObjSafe)
		return pThis->ExternalAddRef();
}


ULONG FAR EXPORT Ctest_activexCtrl::XObjSafe::Release()
{
	METHOD_PROLOGUE(Ctest_activexCtrl, ObjSafe)
		return pThis->ExternalRelease();
}


HRESULT FAR EXPORT Ctest_activexCtrl::XObjSafe::QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(Ctest_activexCtrl, ObjSafe)
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


const DWORD dwSupportedBits = INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA;
const DWORD dwNotSupportedBits=~dwSupportedBits;


//   CStopLiteCtrl::XObjSafe::GetInterfaceSafetyOptions   
//   Allows   container   to   query   what   interfaces   are   safe   for   what.   We're   
//   optimizing   significantly   by   ignoring   which   interface   the   caller   is   
//   asking   for.   
HRESULT STDMETHODCALLTYPE     
	Ctest_activexCtrl::XObjSafe::GetInterfaceSafetyOptions(     
	/*   [in]   */   REFIID   riid,   
	/*   [out]   */   DWORD   __RPC_FAR   *pdwSupportedOptions,   
	/*   [out]   */   DWORD   __RPC_FAR   *pdwEnabledOptions)   
{   
	METHOD_PROLOGUE(Ctest_activexCtrl, ObjSafe)   
		HRESULT   retval   =   ResultFromScode(S_OK);   
	//   does   interface   exist?   
	IUnknown FAR* punkInterface;   
	retval = pThis->ExternalQueryInterface(&riid, (void **)&punkInterface);   
	if(retval!=E_NOINTERFACE) 
	{ //   interface   exists   
		punkInterface->Release();   //   release   it--just   checking!   
	}   
	//   we   support   both   kinds   of   safety   and   have   always   both   set, regardless   of   interface   
	*pdwSupportedOptions =* pdwEnabledOptions=dwSupportedBits;   
	return retval;   //   E_NOINTERFACE   if   QI   failed   
}  


/////////////////////////////////////////////////////////////////////////////   
//   CStopLiteCtrl::XObjSafe::SetInterfaceSafetyOptions   
//   Since   we're   always   safe,   this   is   a   no-brainer--but   we   do   check   to   make   
//   sure   the   interface   requested   exists   and   that   the   options   we're   asked   to   
//   set   exist   and   are   set   on   (we   don't   support   unsafe   mode).   
HRESULT STDMETHODCALLTYPE     
	Ctest_activexCtrl::XObjSafe::SetInterfaceSafetyOptions(     
	/*   [in]   */   REFIID   riid,   
	/*   [in]   */   DWORD   dwOptionSetMask,   
	/*   [in]   */   DWORD   dwEnabledOptions)   
{   
	METHOD_PROLOGUE(Ctest_activexCtrl, ObjSafe)   
		//   does   interface   exist?   
		IUnknown   FAR*   punkInterface;   
	pThis->ExternalQueryInterface(&riid, (void **)&punkInterface);   
	if(punkInterface)
	{ //   interface   exists   
		punkInterface->Release();   //   release   it--just   checking!   
	}   
	else 
	{   //   interface   doesn't   exist   
		return ResultFromScode(E_NOINTERFACE);   
	}  

	//   can't   set   bits   we   don't   support   
	if(dwOptionSetMask & dwNotSupportedBits)  
	{     
		return ResultFromScode(E_FAIL);   
	}  

	//   can't   set   bits   we   do   support   to   zero   
	dwEnabledOptions &= dwSupportedBits;   
	//   (we   already   know   there   are   no   extra   bits   in   mask   )   
	if((dwOptionSetMask & dwEnabledOptions) != dwOptionSetMask) 
	{   
		return ResultFromScode(E_FAIL);   
	}   
	//   don't   need   to   change   anything   since   we're   always   safe   
	return ResultFromScode(S_OK);   
}
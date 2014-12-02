#pragma once

#include ".\DuiLib\UIlib.h"
using namespace DuiLib;


#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, ".\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, ".\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, ".\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, ".\\DuiLib.lib")
#   endif
#endif


// 以XML生成界面的窗口基类
class CXMLWnd : public WindowImplBase
{
public:
    explicit CXMLWnd(LPCTSTR pszXMLPath) 
        : m_strXMLPath(pszXMLPath){}

public:
    virtual LPCTSTR GetWindowClassName() const
    {
        return _T("XMLWnd");
    }

    virtual CDuiString GetSkinFile()
    {
        return m_strXMLPath;
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("");
    }

protected:
    CDuiString m_strXMLPath;
};

#define WM_USER_SIZE			WM_USER + 4     // 播放完毕
// 将HWND显示到CControlUI上面
class CWndUI: public CControlUI
{
public:
    CWndUI(): m_hWnd(NULL)
	{
		player_rc = new RECT();
	}
	~CWndUI()
	{
		if (player_rc)
		{
			delete player_rc;
			player_rc = NULL;
		}
	}

    virtual void SetInternVisible(bool bVisible = true)
    {
        __super::SetInternVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible);
    }

    virtual void SetPos(RECT rc)
    {
        __super::SetPos(rc);
		// TODO：在这里需要重新调整一下播放视频的宽和高
		player_rc->left = rc.left;
		player_rc->top = rc.top;
		player_rc->right = rc.right;
		player_rc->bottom = rc.bottom;

        ::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
		::PostMessage(parent_hwnd_, WM_USER_SIZE, 0, 0); // 通知一下父窗口，当前窗口的大小发生了改变
    }

    BOOL Attach(HWND hWndNew)
    {
        if (! ::IsWindow(hWndNew))
        {
            return FALSE;
        }

        m_hWnd = hWndNew;
        return TRUE;
    }

    HWND Detach()
    {
        HWND hWnd = m_hWnd;
        m_hWnd = NULL;
        return hWnd;
    }

	HWND GetHWND()
	{
		return m_hWnd;
	}

	inline void SetParentHwnd(HWND parent_hwnd)
	{
		parent_hwnd_ = parent_hwnd;
	}

	inline RECT GetPlayerRc()
	{
		return *player_rc;
	}

protected:
    HWND m_hWnd;
	HWND parent_hwnd_;	// 表示他的父窗口，即整个播放器的窗口
	RECT *player_rc;	// 当前播放窗口的信息
};

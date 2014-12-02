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


// ��XML���ɽ���Ĵ��ڻ���
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

#define WM_USER_SIZE			WM_USER + 4     // �������
// ��HWND��ʾ��CControlUI����
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
		// TODO����������Ҫ���µ���һ�²�����Ƶ�Ŀ�͸�
		player_rc->left = rc.left;
		player_rc->top = rc.top;
		player_rc->right = rc.right;
		player_rc->bottom = rc.bottom;

        ::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
		::PostMessage(parent_hwnd_, WM_USER_SIZE, 0, 0); // ֪ͨһ�¸����ڣ���ǰ���ڵĴ�С�����˸ı�
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
	HWND parent_hwnd_;	// ��ʾ���ĸ����ڣ��������������Ĵ���
	RECT *player_rc;	// ��ǰ���Ŵ��ڵ���Ϣ
};

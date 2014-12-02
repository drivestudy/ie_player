#include "time.h"

#include "DuiFrameWnd.h"
#include "MenuWnd.h"


#define WM_USER_PLAYING         WM_USER + 1     // 开始播放文件
#define WM_USER_POS_CHANGED     WM_USER + 2     // 文件播放位置改变
#define WM_USER_END_REACHED     WM_USER + 3     // 播放完毕
#define WM_USER_SIZE			WM_USER + 4     // 播放完毕


// 文件筛选
const TCHAR STR_FILE_FILTER[] =
	_T("All Files(*.*)\0*.*\0")
	_T("Movie Files(*.rmvb,*.mpeg,etc)\0*.rm;*.rmvb;*.flv;*.f4v;*.avi;*.3gp;*.mp4;*.wmv;*.mpeg;*.mpga;*.asf;*.dat;*.mov;*.dv;*.mkv;*.mpg;*.trp;*.ts;*.vob;*.xv;*.m4v;*.dpg;\0")
	_T("Music Files(*.mp3,*.wma,etc)\0*.mp3;*.wma;*.wav;*.mid;*.rmi;*.aac;*.ac3;*.aiff;*.amr;*.m4a;*.mka;*.mp2;*.ogg;*.ra;*.au;*.flac;\0");



void CallbackPlayer(void *data, UINT uMsg)
{
	CAVPlayer *pAVPlayer = (CAVPlayer *) data;

	if (pAVPlayer)
	{
		HWND hWnd = pAVPlayer->GetHWND();

		if (::IsWindow(hWnd) && ::IsWindow(::GetParent(hWnd)))
		{
			::PostMessage(::GetParent(hWnd), uMsg, (WPARAM)data, 0);
		}
	}
}


void CDuiFrameWnd::InitWindow()
{
 //   CenterWindow();

	// 根据分辨率自动调节窗口大小
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
	AdaptWindowSize(oMonitor.rcMonitor.right - oMonitor.rcMonitor.left);
	::GetWindowPlacement(*this, &old_windows_placement_);

	// 初始化CActiveXUI控件
	std::vector<CDuiString> vctName;
	CActiveXUI* pActiveXUI;

	vctName.push_back(_T("ActiveXLib"));
	vctName.push_back(_T("ActiveXFind"));
	vctName.push_back(_T("ActiveXMine"));
	vctName.push_back(_T("ActiveXCloud"));

	for (UINT i = 0; i < vctName.size(); i++)
	{
		pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(vctName[i]));

		if(pActiveXUI) 
		{
			pActiveXUI->SetDelayCreate(false);                     
			pActiveXUI->CreateControl(CLSID_WebBrowser);    
		}
	}

	// 几个常用控件做为成员变量
	CSliderUI *pSilderVol = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderVol")));
	slider_ui_ = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderPlay")));
	time_  = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelPlayTime")));

	if (! pSilderVol || ! slider_ui_ || ! time_)
	{
		return;
	}

	pSilderVol->OnNotify    += MakeDelegate(this, &CDuiFrameWnd::OnVolumeChanged);
	slider_ui_->OnNotify += MakeDelegate(this, &CDuiFrameWnd::OnPosChanged);

	// 设置播放器的窗口句柄和回调函数
	CWndUI *pWnd = static_cast<CWndUI*>(m_PaintManager.FindControl(_T("wndMedia")));
	if (pWnd)
	{
		avplayer_->SetHWND(pWnd->GetHWND());
		pWnd->SetParentHwnd(*this);
	}

	// 加载m3u播放列表
//	AddFiles(m_cPlayList.GetPlaylist(), true);   

	// 选中上一次播放文件的位置
	CTreeViewUI *pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treePlaylist")));
	if (pTree)
	{
		pTree->SelectItem(play_list_index_, true);
	}
}


void CDuiFrameWnd::Notify(TNotifyUI& msg)
{
// 	if( msg.sType == DUI_MSGTYPE_ITEMACTIVATE )   
// 	{
// 		CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treePlaylist")));
// 
// 		if(pTree && -1 != pTree->GetItemIndex(msg.pSender) && U_TAG_PLAYLIST == msg.pSender->GetTag())
// 		{
// 			m_iPlaylistIndex = pTree->GetItemIndex(msg.pSender);          
// 			Play(m_cPlayList.GetPlaylist(GetPlaylistIndex(m_iPlaylistIndex)).c_str());  //(static_cast<CTreeNodeUI*> (msg.pSender))->GetItemText();
// 		}
// 	}

	/*
	else if(msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		CDuiString    strName = msg.pSender->GetName();
		CTabLayoutUI* pTab    = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabCaption")));
		std::vector<CDuiString> vctString;

		vctString.push_back(_T("tabPlay"));
		vctString.push_back(_T("tabLib"));
		vctString.push_back(_T("tabFind"));
		vctString.push_back(_T("tabMine"));
		vctString.push_back(_T("tabCloud"));

		std::vector<CDuiString>::iterator it = std::find(vctString.begin(), vctString.end(), strName);
		if (vctString.end() != it)
		{
			int iIndex = it - vctString.begin();
			pTab->SelectItem(iIndex);

			// 加载网页
			// 由于加载网页会耗很多内存，所以这里选择动态加载
			if (iIndex > 0)
			{
				std::vector<CDuiString> vctName, vctURL;
				CActiveXUI* pActiveXUI;

				vctName.push_back(_T("ActiveXLib"));
				vctName.push_back(_T("ActiveXFind"));
				vctName.push_back(_T("ActiveXMine"));
				vctName.push_back(_T("ActiveXCloud"));

				vctURL.push_back(_T("http://pianku.xmp.kankan.com/moviestore_index.html"));
				vctURL.push_back(_T("http://search.xmp.kankan.com/index4xmp.shtml"));
				vctURL.push_back(_T("http://pianku.xmp.kankan.com/xmpguess/host.html"));
				vctURL.push_back(_T("http://vod.xunlei.com/page/xmp/home/home.html?init=1"));

				iIndex--;
				pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(vctName[iIndex]));

				if(pActiveXUI) 
				{
					IWebBrowser2* pWebBrowser = NULL;
					pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);

					if(pWebBrowser) 
					{
						_bstr_t bstrTmp;
						BSTR    bstr;

						pWebBrowser->get_LocationURL(&bstr);
						bstrTmp.Attach(bstr);

						if (! bstrTmp.length())
						{
							pWebBrowser->Navigate(_bstr_t(vctURL[iIndex]), NULL,NULL,NULL,NULL);
							pWebBrowser->Release();
						}
					}
				}
			}
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		CDuiString strName = msg.pSender->GetName();

		if (strName == _T("menuSequence"))
		{
			m_emPlayMode = EM_PLAY_MODE_SEQUENCE;
		} 
		else if (strName == _T("menuRandom"))
		{
			m_emPlayMode = EM_PLAY_MODE_RANDOM;
		}
		else if (strName == _T("menuSingleCircle"))
		{
			m_emPlayMode = EM_PLAY_MODE_SINGLE_CIRCLE;
		}
	}
	else if( msg.sType == DUI_MSGTYPE_DBCLICK )   
	{
		if (IsInStaticControl(msg.pSender))
		{
			// 这里会传进来很多次双击消息，所以只获取祖先控件的消息
			if (! msg.pSender->GetParent())
			{
				FullScreen(! m_bFullScreenMode);
			}
		}
	}
	
	if( msg.sType == DUI_MSGTYPE_DBCLICK )   
	{
		if (IsInStaticControl(msg.pSender))
		{
			// 这里会传进来很多次双击消息，所以只获取祖先控件的消息
			if (!msg.pSender->GetParent())
			{
				FullScreen(!m_bFullScreenMode);
			}
		}
	}
	*/
	__super::Notify(msg);
}


LRESULT CDuiFrameWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);

	int i = WM_USER_PLAYING;
	int j = uMsg;

	switch (uMsg)
	{
 		HANDLE_MSG (*this, WM_DROPFILES, OnDropFiles);
// 		HANDLE_MSG (*this, WM_DISPLAYCHANGE, OnDisplayChange);
// 		HANDLE_MSG (*this, WM_GETMINMAXINFO, OnGetMinMaxInfo);

	case WM_USER_PLAYING:
		return OnPlaying(*this, wParam, lParam);
	case WM_USER_POS_CHANGED:
		return OnPosChanged(*this, wParam, lParam, (enum_ForWardOrBackWard)lParam);
	case WM_USER_END_REACHED:
//		return OnEndReached(*this, wParam, lParam);
		return TRUE;
	case WM_USER_SIZE:
		// TODO：在这里就可以试试对播放器的窗口进行调整
		RECT rc;
		CWndUI *pWnd = static_cast<CWndUI*>(m_PaintManager.FindControl(_T("wndMedia")));
		if (pWnd)
		{
			rc = pWnd->GetPlayerRc();
			avplayer_->ChangeWindowSize(rc);
		}
		return TRUE;
	}

	return lRes;
}


LRESULT CDuiFrameWnd::OnPlaying(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}


/*
CControlUI* CDuiFrameWnd::CreateControl(LPCTSTR pstrClassName)
{
    if (_tcsicmp(pstrClassName, _T("WndPlayPanel")) == 0)
    {
        CDialogBuilder builder;
        CControlUI* pUI = builder.Create(_T("WndPlayPanel.xml")); 
        return pUI;
    }
    else if (_tcsicmp(pstrClassName, _T("Caption")) == 0)
    {
        CDialogBuilder builder;
        CControlUI* pUI = builder.Create(_T("Caption.xml")); 
        return pUI;
    }
// 	else if (_tcsicmp(pstrClassName, _T("Playlist")) == 0)
// 	{
// 		CDialogBuilder builder;
// 		CControlUI* pUI = builder.Create(_T("Playlist.xml")); 
// 		return pUI;
// 	}

    return NULL;
}
*/


CControlUI* CDuiFrameWnd::CreateControl( LPCTSTR pstrClassName )
{
	CDuiString     strXML;
	CDialogBuilder builder;

	if (_tcsicmp(pstrClassName, _T("Caption")) == 0)
	{
		strXML = _T("Caption.xml");
	}
	else if (_tcsicmp(pstrClassName, _T("PlayPanel")) == 0)
	{
		strXML = _T("PlayPanel.xml");
	}
	// 去掉不用旁边的playlist了
// 	else if (_tcsicmp(pstrClassName, _T("Playlist")) == 0)
// 	{
// 		strXML = _T("Playlist.xml");
// 	}
	else if (_tcsicmp(pstrClassName, _T("WndMediaDisplay")) == 0)
	{
		CWndUI *pUI = new CWndUI;   
		HWND   hWnd = CreateWindow(_T("#32770"), _T("WndMediaDisplay"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), (HMENU)0, NULL, NULL);
		pUI->Attach(hWnd);  
		return pUI;
	}

	if (! strXML.IsEmpty())
	{
		CControlUI* pUI = builder.Create(strXML.GetData(), NULL, NULL, &m_PaintManager, NULL); // 这里必须传入m_PaintManager，不然子XML不能使用默认滚动条等信息。
		return pUI;
	}

	return NULL;
}


// 点击进度条发生的事件
bool CDuiFrameWnd::OnPosChanged( void* param )
{
	TNotifyUI* pMsg = (TNotifyUI*)param;

	if( pMsg->sType == _T("valuechanged") )
	{
		avplayer_->SeekTo((static_cast<CSliderUI*>(pMsg->pSender))->GetValue() + 1); // 获取的值少了1，导致设置的值也少了1，所以这里+1
	}

	return true;
}


// 这个就是进度条自己走动的函数----应该让定时器去调用
LRESULT CDuiFrameWnd::OnPosChanged(HWND hwnd, WPARAM wParam, LPARAM lParam, enum_ForWardOrBackWard key_down)
{
	CDuiString time;
	struct tm tm_total, tm_current;
	time_t time_total   = slider_ui_->GetMaxValue();
	time_t time_current = slider_ui_->GetValue();
	char sz_total[MAX_PATH], sz_current[MAX_PATH];
	char *p, *q;

	gmtime_s(&tm_total, &time_total);
	gmtime_s(&tm_current, &time_current);

	_tcsftime(sz_total, MAX_PATH, _T("%X"), &tm_total);
	if (sz_total[1] == '0')
	{
		p = sz_total + 3;
	}
	else
	{
		p = sz_total;
	}

	_tcsftime(sz_current, MAX_PATH, _T("%X"), &tm_current);
	if (sz_current[1] == '0')
	{
		q = sz_current + 3;
	}
	else
	{
		q = sz_current;
	}

	time.Format(_T("%s / %s"), q, p);
	time_->SetText(time);

	int slider_play_pos = slider_ui_->GetValue();
	switch (key_down)
	{
	case EM_NULL:
		//	m_pLabelTime->SetText(strTime);
		//	m_pSliderPlay->SetValue(m_cAVPlayer->GetPos());
	//	int i = m_pSliderPlay->GetValue();
		slider_ui_->SetValue(slider_ui_->GetValue() + 1);
		break;
	case EM_FOR_WARD:
		slider_ui_->SetValue(slider_ui_->GetValue() + 5);
		break;
	case EM_BACK_WARD:
		if (slider_play_pos < 5)
		{
			slider_ui_->SetValue(0);
		}
		else
		{
			slider_ui_->SetValue(slider_play_pos - 5);
		}	
		break;
	default:
		break;
	}

	return TRUE;
}


bool CDuiFrameWnd::OnVolumeChanged(void* param)
{
	TNotifyUI *pMsg = (TNotifyUI*)param;

	// TODO：在这里是滚动条滚动的事件
	if( pMsg->sType == _T("valuechanged") )
	{
		avplayer_->Volume((static_cast<CSliderUI*>(pMsg->pSender))->GetValue());
	}

	return true;
}


void CDuiFrameWnd::AdaptWindowSize( UINT cxScreen )
{
	int iX = 968, iY = 600;
	int iWidthList = 225, iWidthSearchEdit = 193;
	SIZE szFixSearchBtn = {201, 0};

	if(cxScreen <= 1024)      // 800*600  1024*768  
	{
		iX = 775;
		iY = 470;
	} 
	else if(cxScreen <= 1280) // 1152*864  1280*800  1280*960  1280*1024
	{
		iX = 968;
		iY = 600;
	}
	else if(cxScreen <= 1366) // 1360*768 1366*768
	{
		iX = 1058;
		iY = 656;
		iWidthList        += 21;
		iWidthSearchEdit  += 21;
		szFixSearchBtn.cx += 21;
	}
	else                      // 1440*900
	{
		iX = 1224;
		iY = 760;
		iWidthList        += 66;
		iWidthSearchEdit  += 66;
		szFixSearchBtn.cx += 66;
	}

	CControlUI *pctnPlaylist = m_PaintManager.FindControl(_T("ctnPlaylist"));
	CControlUI *peditSearch  = m_PaintManager.FindControl(_T("editSearch"));
	CControlUI *pbtnSearch   = m_PaintManager.FindControl(_T("btnSearch"));
	if (pctnPlaylist && peditSearch && pbtnSearch)
	{
		pctnPlaylist->SetFixedWidth(iWidthList);
		peditSearch->SetFixedWidth(iWidthSearchEdit);
		pbtnSearch->SetFixedXY(szFixSearchBtn);
	}

	::SetWindowPos(m_PaintManager.GetPaintWindow(), NULL, 0, 0, iX, iY, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOACTIVATE);
//	CenterWindow();
}


void CDuiFrameWnd::OnDropFiles(HWND hwnd, HDROP hDropInfo)
{
	UINT  nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	TCHAR szFileName[_MAX_PATH] = _T("");
	DWORD dwAttribute;

	// 获取拖拽进来文件和文件夹
	for (UINT i = 0; i < nFileCount; i++)
	{
		::DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
		dwAttribute = ::GetFileAttributes(szFileName);

		// 是否为文件夹
		if (dwAttribute & FILE_ATTRIBUTE_DIRECTORY)
		{          
// 			::SetCurrentDirectory(szFileName);
// 			EnumerateFiles(vctString);
			// 暂时不要去管文件夹
		}
		else
		{
			// 文件可以直接进行播放
			Play(szFileName);
		}
	}

	::DragFinish(hDropInfo);
}


void CDuiFrameWnd::Play(bool bPlay)
{
	if (avplayer_->GetPlayStatus() == MPLAYER_BEGIN)
	{
		Play(file_path_.c_str());
	}
	else
	{
		avplayer_->Pause();
	}

	ShowPlayButton(!bPlay);

	// 检查有没有在暂停的时候，按下进度条的点
	if (avplayer_->GetPauseSeekToPos() != 0)
	{
		// 首先跳转
		avplayer_->SeekTo(avplayer_->GetPauseSeekToPos());
		avplayer_->SetPauseSeekToPos(0);
	}
}


void CDuiFrameWnd::Play( LPCTSTR pszPath )
{
	if (!file_path_.compare(""))
	{
		return;
	}

	// 如果已经正在播放，那么首先停止
	if (avplayer_->GetPlayStatus() == MPLAYER_PLAYING || 
		avplayer_->GetPlayStatus() == MPLAYER_PAUSED  ||
		avplayer_->GetPlayStatus() == MPLAYER_STARTING)
	{
		// 停止
		avplayer_->Stop();
	}

	play_file_path_ = pszPath;

	if (avplayer_->Play(file_path_))
	{
		ShowControlsForPlay(true);
	}

	// 播放开始之后，要根据影片的大小进行修改
	::PostMessage(parent_hwnd_, WM_SIZE, 0, 0);

	slider_ui_->SetMinValue(0);
	long i = avplayer_->GetTotalTime();
	slider_ui_->SetMaxValue(i + 8); // 需要加上冗余
	slider_ui_->SetValue(0);

	is_playing_ = true;

	// 添加到播放记录中
	play_list_.AddPlayVideoPath(pszPath);
}


void CDuiFrameWnd::Stop()
{
	avplayer_->Stop();
	ShowControlsForPlay(false);

	is_playing_ = false;
}


void CDuiFrameWnd::ShowPlayButton(bool bShow)
{
	CControlUI *pbtnPlay  = m_PaintManager.FindControl(_T("btnPlay"));
	CControlUI *pbtnPause = m_PaintManager.FindControl(_T("btnPause"));

	if (pbtnPlay && pbtnPause)
	{
		pbtnPlay->SetVisible(bShow);
		pbtnPause->SetVisible(!bShow);
	}
}


void CDuiFrameWnd::ShowPlayWnd( bool bShow )
{
	CControlUI *pbtnWnd     = m_PaintManager.FindControl(_T("wndMedia"));
	CControlUI *pbtnStop    = m_PaintManager.FindControl(_T("btnStop"));
	CControlUI *pbtnScreen  = m_PaintManager.FindControl(_T("btnScreenFull"));
	CControlUI *pctnURL     = m_PaintManager.FindControl(_T("ctnURL"));
	CControlUI *pctnClient  = m_PaintManager.FindControl(_T("ctnClient"));
	CControlUI *pctnMusic   = m_PaintManager.FindControl(_T("ctnMusic"));
	CControlUI *pctnSlider  = m_PaintManager.FindControl(_T("ctnSlider"));

	if (pbtnWnd && pbtnStop && pbtnScreen && pctnURL && pctnClient && pctnMusic && pctnSlider)
	{
		pbtnStop->SetEnabled(bShow);
		pbtnScreen->SetEnabled(bShow);
		pctnURL->SetVisible(! bShow);
		pctnClient->SetVisible(! bShow);
		pctnSlider->SetVisible(bShow);

		// 打开文件时
		if (bShow)  
		{
// 			if (IsMusicFile(m_strPath))
// 			{
// 				pbtnWnd->SetVisible(! bShow);
// 				pctnMusic->SetVisible(bShow);
// 			} 
// 			else
// 			{
				pbtnWnd->SetVisible(bShow);
				pctnMusic->SetVisible(!bShow);
//			}
		}
		// 关闭文件时
		else        
		{
			pctnMusic->SetVisible(false);
			pbtnWnd->SetVisible(false);
		}
	}
}


void CDuiFrameWnd::ShowControlsForPlay( bool bShow )
{
	time_->SetText(_T(""));
	ShowPlayWnd(bShow);
	ShowPlaylist(!bShow);
	ShowPlayButton(!bShow);
}


void CDuiFrameWnd::ShowPlaylist( bool bShow )
{
	CControlUI *pctnPlaylist = m_PaintManager.FindControl(_T("ctnPlaylist"));
	CControlUI *pbtnHide     = m_PaintManager.FindControl(_T("btnPlaylistHide"));
	CControlUI *pbtnShow     = m_PaintManager.FindControl(_T("btnPlaylistShow"));

	if (pctnPlaylist && pbtnHide && pbtnShow)
	{
		pctnPlaylist->SetVisible(bShow);
		pbtnHide->SetVisible(bShow);
		pbtnShow->SetVisible(! bShow);
	}
}


void CDuiFrameWnd::OnClick( TNotifyUI& msg )
{
	if( msg.pSender->GetName() == _T("btnPlaylistShow") ) 
	{
		ShowPlaylist(true);
	}
	else if( msg.pSender->GetName() == _T("btnPlaylistHide") ) 
	{
		ShowPlaylist(false);
	}
	if( msg.pSender->GetName() == _T("btnFastBackward") ) 
	{
		avplayer_->SeekBackward();
		::PostMessage(*this, WM_USER_POS_CHANGED, 0, enum_ForWardOrBackWard::EM_BACK_WARD);
	}
	else if( msg.pSender->GetName() == _T("btnFastForward") ) 
	{
		avplayer_->SeekForward();
		::PostMessage(*this, WM_USER_POS_CHANGED, 0, enum_ForWardOrBackWard::EM_FOR_WARD);
	}
// 	else if( msg.pSender->GetName() == _T("btnPrevious") ) 
// 	{
// 		Play(GetNextPath(false));
// 	}
// 	else if( msg.pSender->GetName() == _T("btnNext") ) 
// 	{
// 		Play(GetNextPath(true));
// 	}
	else if( msg.pSender->GetName() == _T("btnPlay") ) 
	{
		Play(true);
	}
	else if( msg.pSender->GetName() == _T("btnPause") ) 
	{
		Play(false);
	}
	else if( msg.pSender->GetName() == _T("btnStop") ) 
	{
		Stop();
	}
	else if( msg.pSender->GetName() == _T("btnOpen") || msg.pSender->GetName() == _T("btnOpenMini") ) 
	{
		OpenFileDialog(); 
	}
	else if( msg.pSender->GetName() == _T("btnScreenFull") ) 
	{
		FullScreen(true);
	}
	else if( msg.pSender->GetName() == _T("btnScreenNormal") ) 
	{
		FullScreen(false);
	}
	else if( msg.pSender->GetName() == _T("btnVolume") ) 
	{
		avplayer_->Volume(0);
		m_PaintManager.FindControl(_T("btnVolumeZero"))->SetVisible(true);
		msg.pSender->SetVisible(false);
	}
	else if( msg.pSender->GetName() == _T("btnVolumeZero") ) 
	{
		CSliderUI* pUI = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderVol")));
		avplayer_->Volume(pUI->GetValue());
		m_PaintManager.FindControl(_T("btnVolume"))->SetVisible(true);
		msg.pSender->SetVisible(false);
	}
	else if( msg.pSender->GetName() == _T("btnPlayMode") ) 
	{
		CMenuWnd *pMenu = new CMenuWnd(_T("menu.xml"));
		POINT    pt = {msg.ptMouse.x, msg.ptMouse.y};
		CDuiRect rc = msg.pSender->GetPos();

		pt.x = rc.left;
		pt.y = rc.bottom;
		pMenu->Init(&m_PaintManager, pt);
		pMenu->ShowWindow(TRUE);
	}

	__super::OnClick(msg);
}


void CDuiFrameWnd::FullScreen( bool bFull )
{
	CControlUI* pbtnFull   = m_PaintManager.FindControl(_T("btnScreenFull"));
	CControlUI* pbtnNormal = m_PaintManager.FindControl(_T("btnScreenNormal"));
	CControlUI* pUICaption = m_PaintManager.FindControl(_T("ctnCaption"));
	int iBorderX = GetSystemMetrics(SM_CXFIXEDFRAME) + GetSystemMetrics(SM_CXBORDER);
	int iBorderY = GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYBORDER);

	if (pbtnFull && pbtnNormal && pUICaption)
	{
		is_full_screen_mode_ = bFull;

		if (bFull)
		{
			::GetWindowPlacement(*this, &old_windows_placement_);

			if (::IsZoomed(*this))
			{
				::ShowWindow(*this, SW_SHOWDEFAULT);
			}

			::SetWindowPos(*this, HWND_TOPMOST, -iBorderX, -iBorderY, GetSystemMetrics(SM_CXSCREEN) + 2 * iBorderX, GetSystemMetrics(SM_CYSCREEN) + 2 * iBorderY, 0);
		//	ShowPlaylist(false);
		} 
		else
		{
			::SetWindowPlacement(*this, &old_windows_placement_);
			::SetWindowPos(*this, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}

		pbtnNormal->SetVisible(bFull);
		pUICaption->SetVisible(! bFull);
		pbtnFull->SetVisible(! bFull);
	}
}


void CDuiFrameWnd::OpenFileDialog()
{
	OPENFILENAME ofn;      
	TCHAR szFile[MAX_PATH] = _T("");

	ZeroMemory(&ofn, sizeof(ofn));  
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner   = *this;
	ofn.lpstrFile   = szFile;   
	ofn.nMaxFile    = sizeof(szFile);  
	ofn.lpstrFilter = STR_FILE_FILTER;
	ofn.nFilterIndex    = 1;  
	ofn.lpstrFileTitle  = NULL;  
	ofn.nMaxFileTitle   = 0;  
	ofn.lpstrInitialDir = NULL;  
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  

	if (GetOpenFileName(&ofn))  
	{
//		std::vector<string> vctString(1, szFile);
//		AddFiles(vctString);
	//	file_path_ = szFile;
		file_path_ = szFile;

		// 恢复已经根据视频宽高修改对应的播放窗口的标识
		avplayer_->SetIsAlreadyChangeWindows(false);

		Play(szFile);
	}
}

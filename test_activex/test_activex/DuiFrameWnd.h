#ifndef  __DUI_FRAME_WND_H__
#define  __DUI_FRAME_WND_H__


#include "duilib.h"
#include "AVPlayer.h"
#include "PlayList.h"

#include "shellapi.h"


const TCHAR STR_PATH_PLAYLIST[] = _T("PlayNote.m3u");


enum enum_PlayMode
{
	EM_PLAY_MODE_SEQUENCE,      // 顺序播放
	EM_PLAY_MODE_RANDOM,        // 随机播放
	EM_PLAY_MODE_SINGLE_CIRCLE  // 单曲循环
};

enum enum_ForWardOrBackWard
{
	EM_NULL,		// 没有按下快进快退
	EM_FOR_WARD,    // 随机播放
	EM_BACK_WARD	// 单曲循环
};


class CDuiFrameWnd: public CXMLWnd
{
public:
	explicit CDuiFrameWnd::CDuiFrameWnd(LPCTSTR pszXMLPath)
		: CXMLWnd(pszXMLPath)
	{
		is_playing_  = false;
		parent_hwnd_ = NULL;
		file_path_   = "";
		avplayer_  = new CAVPlayer;

		video_windows_width_  = 0;
		video_windows_height_ = 0;

		// 读取配置文件信息
	//	play_list_.ReadFile((CPaintManagerUI::GetInstancePath() + STR_PATH_PLAYLIST).GetData());
	}

	~CDuiFrameWnd()
	{
		if (avplayer_)
		{
			delete avplayer_;
			avplayer_ = NULL;
		}
	//	play_list_.WriteFile((CPaintManagerUI::GetInstancePath() + STR_PATH_PLAYLIST).GetData());
	}

	//	DUI_DECLARE_MESSAGE_MAP()
	virtual void InitWindow();
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	void CDuiFrameWnd::Notify( TNotifyUI& msg );
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnPlaying(HWND hwnd, WPARAM wParam, LPARAM lParam);     // 文件头读取完毕，开始播放

	bool    OnPosChanged(void* param);                              // 进度改变，用户主动改变进度
	LRESULT OnPosChanged(HWND hwnd, WPARAM wParam, LPARAM lParam, enum_ForWardOrBackWard key_down);  // 进度改变，播放器传回来的进度
	bool    OnVolumeChanged(void* param);                           // 音量改变

	void OnDropFiles(HWND hwnd, HDROP hDropInfo);

	void Play(LPCTSTR pszPath);                                     // 播放路径为pszPath的文件
	void Play(bool bPlay);                                          // 播放或暂停
	void Stop();                                                    // 停止
	virtual void OnClick(TNotifyUI& msg);
	void OpenFileDialog();                                          // 打开文件窗口
	void ShowPlaylist(bool bShow);                                  // 显示播放列表

	inline bool GetIsPlayStatus()
	{
		return is_playing_;
	}

	inline void SetParentHwnd(HWND parent_hwnd)
	{
		parent_hwnd_ = parent_hwnd;
	}

	inline int GetVideoWidth()
	{
		return avplayer_->GetVideoWidth();
	}

	inline int GetVideoHeight()
	{
		return avplayer_->GetVideoHeight();
	}

	inline void SetFilePath(string file_path)
	{
		file_path_ = file_path;
	}

	inline void SetVideoWindowsWidth(int video_windows_width)
	{
		video_windows_width_ = video_windows_width;
	}

	inline void SetVideoWindowsHeight(int video_windows_height)
	{
		video_windows_height_ = video_windows_height;
	}

	inline int GetPlayerWindowsWidth()
	{
		return avplayer_->GetPlayerWindowsWidth();
	}
	inline void SetPlayerWindowsWidth(int player_windows_width)
	{
		avplayer_->SetPlayerWindowsWidth(player_windows_width);
	}

	inline int GetPlayerWindowsHeight()
	{
		return avplayer_->GetPlayerWindowsHeight();
	}
	inline void SetPlayerWindowsHeight(int player_windows_height)
	{
		avplayer_->SetPlayerWindowsHeight(player_windows_height);
	}

	inline bool GetIsAlreadyChangeWindows()
	{
		return avplayer_->GetIsAlreadyChangeWindows();
	}
	inline void SetIsAlreadyChangeWindows(bool is_already_change_windows)
	{
		avplayer_->SetIsAlreadyChangeWindows(is_already_change_windows);
	}

	inline bool GetIsAlreadyObtainVideoInfo()
	{
		return avplayer_->GetIsAlreadyObtainVideoInfo();
	}

	inline void ResetAVPlayer()
	{
		if (avplayer_)
		{
			delete avplayer_;
			avplayer_ = NULL;
		}
	}

private:
	void AdaptWindowSize(UINT cxScreen);        // 根据屏幕分辨率自动调整窗口大小
	void ShowPlayButton(bool bShow);            // 显示播放按钮
	void ShowPlayWnd(bool bShow);               // 显示播放窗口
	void ShowControlsForPlay(bool bShow);       // 当开始播放或停止时，显示或隐藏一些控件
	void FullScreen(bool bFull);                // 全屏

private:
	CAVPlayer       *avplayer_;					// 播放器类
	CDuiString      play_file_path_;			// 当前文件的路径
	CSliderUI       *slider_ui_;				// 文件播放进度
	CLabelUI        *time_;						// 文件播放时间
	WINDOWPLACEMENT old_windows_placement_;		// 保存窗口原来的位置
	bool            is_full_screen_mode_;		// 是否在全屏模式
	int             play_list_index_;			// 当前播放文件在tree控件里的下标  treeview控件目前很不完善，所以用m_iPlaylistIndex来辅助标记
	enum_PlayMode   play_mode_;					// 播放模式
	string			file_path_;					// 文件的路径
	bool			is_playing_;				// 是否正在播放
	HWND			parent_hwnd_;				// 如果是嵌入，那么这个变量就是父窗口的句柄
	PlayList		play_list_;					// 用来保存播放记录

	// 这两个以后留着备用
	int video_windows_width_;					// 用来记录窗口的宽度
	int video_windows_height_;					// 用来记录窗口的高度
};


#endif // end if __DUI_FRAME_WND_H__
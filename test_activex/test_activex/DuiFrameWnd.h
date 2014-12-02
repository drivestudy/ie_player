#ifndef  __DUI_FRAME_WND_H__
#define  __DUI_FRAME_WND_H__


#include "duilib.h"
#include "AVPlayer.h"
#include "PlayList.h"

#include "shellapi.h"


const TCHAR STR_PATH_PLAYLIST[] = _T("PlayNote.m3u");


enum enum_PlayMode
{
	EM_PLAY_MODE_SEQUENCE,      // ˳�򲥷�
	EM_PLAY_MODE_RANDOM,        // �������
	EM_PLAY_MODE_SINGLE_CIRCLE  // ����ѭ��
};

enum enum_ForWardOrBackWard
{
	EM_NULL,		// û�а��¿������
	EM_FOR_WARD,    // �������
	EM_BACK_WARD	// ����ѭ��
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

		// ��ȡ�����ļ���Ϣ
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
	LRESULT OnPlaying(HWND hwnd, WPARAM wParam, LPARAM lParam);     // �ļ�ͷ��ȡ��ϣ���ʼ����

	bool    OnPosChanged(void* param);                              // ���ȸı䣬�û������ı����
	LRESULT OnPosChanged(HWND hwnd, WPARAM wParam, LPARAM lParam, enum_ForWardOrBackWard key_down);  // ���ȸı䣬�������������Ľ���
	bool    OnVolumeChanged(void* param);                           // �����ı�

	void OnDropFiles(HWND hwnd, HDROP hDropInfo);

	void Play(LPCTSTR pszPath);                                     // ����·��ΪpszPath���ļ�
	void Play(bool bPlay);                                          // ���Ż���ͣ
	void Stop();                                                    // ֹͣ
	virtual void OnClick(TNotifyUI& msg);
	void OpenFileDialog();                                          // ���ļ�����
	void ShowPlaylist(bool bShow);                                  // ��ʾ�����б�

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
	void AdaptWindowSize(UINT cxScreen);        // ������Ļ�ֱ����Զ��������ڴ�С
	void ShowPlayButton(bool bShow);            // ��ʾ���Ű�ť
	void ShowPlayWnd(bool bShow);               // ��ʾ���Ŵ���
	void ShowControlsForPlay(bool bShow);       // ����ʼ���Ż�ֹͣʱ����ʾ������һЩ�ؼ�
	void FullScreen(bool bFull);                // ȫ��

private:
	CAVPlayer       *avplayer_;					// ��������
	CDuiString      play_file_path_;			// ��ǰ�ļ���·��
	CSliderUI       *slider_ui_;				// �ļ����Ž���
	CLabelUI        *time_;						// �ļ�����ʱ��
	WINDOWPLACEMENT old_windows_placement_;		// ���洰��ԭ����λ��
	bool            is_full_screen_mode_;		// �Ƿ���ȫ��ģʽ
	int             play_list_index_;			// ��ǰ�����ļ���tree�ؼ�����±�  treeview�ؼ�Ŀǰ�ܲ����ƣ�������m_iPlaylistIndex���������
	enum_PlayMode   play_mode_;					// ����ģʽ
	string			file_path_;					// �ļ���·��
	bool			is_playing_;				// �Ƿ����ڲ���
	HWND			parent_hwnd_;				// �����Ƕ�룬��ô����������Ǹ����ڵľ��
	PlayList		play_list_;					// �������沥�ż�¼

	// �������Ժ����ű���
	int video_windows_width_;					// ������¼���ڵĿ��
	int video_windows_height_;					// ������¼���ڵĸ߶�
};


#endif // end if __DUI_FRAME_WND_H__
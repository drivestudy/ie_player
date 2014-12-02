// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Build:   Oct/14/2013
// Author:  Alberl Lee
// Email:   ItsBird@qq.com
// Website: http://www.cnblogs.com/Alberl/p/3381820.html
//
// ��ܰ��ʾ��
// ������ÿ�춼�յ��ܶ������ʼ����ʼ����벻Ҫ������ȡ��Ӧ�����������ʼ����ֿ������Һ�QQ�Ĺ��˹��ܻ���ǿ��~O(��_��)O~
// ���������������ָ���ͼ��������������Ǽ���������ֱ�����������������ʣ�����ˡ���ظ�����������Ϊ���ú����С���Ҳ�ܿ����������~O(��_��)O~
//
// ʹ��Э�飺WTFPL
// �������쳯����Э�鶼�����ӣ������ܶ��˾�������أ������Բ����ˡ�DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE��Э��~O(��_��)O~
//
// ���밲ȫ�ԣ�
// ����ĿΪʾ����Ŀ��Ϊ�˷����ұ��룬û�������⣬���д��һЩ�򵥵ĺ������ܶ��߼��ж�Ҳֻ�Ǳ�֤����������ʵ��ʹ�������б�֤���밲ȫ~O(��_��)O~
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#ifndef __AVPlayer_H__
#define __AVPlayer_H__

#include "iostream"
#include "string"
#include "Windows.h"


enum MPLAYER_STATUS
{
	MPLAYER_BEGIN,
	MPLAYER_STARTING,
	MPLAYER_PLAYING,
	MPLAYER_STOPPING,
	MPLAYER_STOPPED,
	MPLAYER_PAUSED,
	MPLAYER_ERROR,
};


class CAVPlayer
{
public:
	CAVPlayer(char *video_play_original_key = "");
	~CAVPlayer();

	bool Play(const std::string &strPath);  // ����·��ΪstrPath���ļ�
	void Play();							// ����
	void Pause();                           // ��ͣ
	void Stop();							// ֹͣ

	void Volume(int iVol);                  // ��������ΪiVol

	void SeekTo(int iPos);                  // ����ָ��λ��iPos
	void SeekForward();                     // ���
	void SeekBackward();                    // ����

	void SetHWND(HWND hwnd);                // ������Ƶ��ʾ�Ĵ��ھ��
	HWND GetHWND();                         // ��ȡ��Ƶ��ʾ�Ĵ��ھ��

	bool    IsOpen();                       // �ļ��Ƿ��
	bool    IsPlaying();                    // �ļ��Ƿ����ڲ���
	int     GetPos();                       // ��ȡ�ļ���ǰ���ŵ�λ��
	__int64 GetTotalTime();                 // ��ȡ��ʱ��
	__int64 GetTime();                      // ��ȡʱ��
	int     GetVolume();                    // ��ȡ����
	void ChangeWindowSize(RECT &rc);		// �޸ĵ�ǰ������Ƶ�Ŀ�͸�

	void Reset(void); // when user start play a clip, reset media clip related info
	int  StartSendCmdThread(void);

	inline int GetVideoWidth()
	{
		return video_width_;
	}

	inline int GetVideoHeight()
	{
		return video_height_;
	}

	inline int GetPlayStatus()
	{
		return play_status_;
	}

	// 	inline bool GetIsChoseVideoInfoSize()
	// 	{
	// 		return is_chose_video_info_size_;
	// 	}
	// 	inline void SetIsChoseVideoInfoSize(bool is_chose_video_info_size)
	// 	{
	// 		is_chose_video_info_size_ = is_chose_video_info_size;
	// 	}

	inline int GetPauseSeekToPos()
	{
		return pause_seek_to_pos_;
	}
	inline void SetPauseSeekToPos(int pause_seek_to_pos)
	{
		pause_seek_to_pos_ = pause_seek_to_pos;
	}

	inline int GetPlayerWindowsWidth()
	{
		return player_windows_width_;
	}
	inline void SetPlayerWindowsWidth(int player_windows_width)
	{
		player_windows_width_ = player_windows_width;
	}

	inline void SetPlayerWindowsHeight(int player_windows_height)
	{
		player_windows_height_ = player_windows_height;
	}
	inline int GetPlayerWindowsHeight()
	{
		return player_windows_height_;
	}

	inline bool GetIsAlreadyChangeWindows()
	{
		return is_already_change_windows_;
	}
	inline void SetIsAlreadyChangeWindows(bool is_already_change_windows)
	{
		is_already_change_windows_ = is_already_change_windows;
	}

	inline bool GetIsAlreadyObtainVideoInfo()
	{
		return is_already_obtain_video_info_;
	}
	inline void SetIsAlreadyObtainVideoInfo(bool is_already_get_video_info)
	{
		is_already_obtain_video_info_ = is_already_get_video_info;
	}

private:
	HWND video_hwnd_;						// ��Ƶ��ʾ�Ĵ��ھ��
	char cmd_string_[40];
	BOOL is_allow_to_send_cmd_;
	// Trying to avoid flooded by Pipe crap
	BOOL is_pipe_full_;						// Pipe is full previously
	int pipe_full_count_;					// Number of times pipe is full
	HANDLE thread_send_cmd_string_;
	HANDLE send_cmd_string_, send_cmd_end_;
	int pipe_buffer_size_;

	void Init();                            // ��ʼ��
	void Release();                         // �����ڴ�
	void RequireTotalTime();
	void RequireWidthHeight();

	int CreateCmdPipe();					// ��������ܵ�
	void MakeupCmdLn();						// ����������
	int DestroyCmdPipe();					// ��������ܵ�
	void CleanUp();							// cleanup something after playback
	void TerminateSendCmdThread();
	void GetCmdString(char[]);
	void SeekTime(DWORD dwSeekTime);		// ��λ��һ������λ��(��λ��).

	// thread related
	static DWORD WINAPI SendCmd(LPVOID p);
	void SendCmd();
	int Command(std::string strCommand);	// ͨ���ܵ�ΪMPlayer��������
	void AddArgument(std::string strArgument);//��Ӳ���
	int CheckActivate();					// ���MPlayer�����Ƿ���������

private:
	// ��Ҫ��Ա����
	std::string cmd_param_;					// command line
	std::string play_exe_path_;				// MPlayer Binary file path name
	std::string file_path_;					// source filename, media clip filename

	int play_status_;						// current status, play, stop, pause, ...
	long video_time_length_;				// ӰƬ�ܵĳ���
	long current_time_pos_;

	int video_width_;						// ӰƬ�Ŀ��
	int video_height_;						// ӰƬ�ĸ߶�

	int player_windows_width_;				// ���Ŵ��ڵĿ��
	int player_windows_height_;				// ���Ŵ��ڵĸ߶�

	long last_time_pos_;					// �ϴε�����Ž�������λ��
	int last_volume_pos_;					// �ϴε��ʱ���������λ��
	int pause_seek_to_pos_;					// ��¼��ͣʱ����ȥ�ĵ�
	//	bool is_chose_video_info_size_;		// �Ƿ���Ҫ�޸Ĳ��Ŵ��ڣ�����ӦӰƬ

	bool is_already_obtain_video_info_;		// �Ƿ��Ѿ��õ�����Ƶ�Ŀ�͸�
	bool is_already_change_windows_;		// �Ƿ��Ѿ�������Ƶ����޸Ķ�Ӧ�Ĳ��Ŵ���

	// MPlayer process related member variables
	// Command pipe related member variables
	SECURITY_ATTRIBUTES security_arrtibutes_;
	HANDLE write_to_child_;
	HANDLE read_from_child_;
	HANDLE write_to_parent_;
	HANDLE read_from_parent_;

	// MPlayer process related member variables
	STARTUPINFO startupinfo_;
	PROCESS_INFORMATION process_infomation_;
};


#endif

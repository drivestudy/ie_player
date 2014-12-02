// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Build:   Oct/14/2013
// Author:  Alberl Lee
// Email:   ItsBird@qq.com
// Website: http://www.cnblogs.com/Alberl/p/3381820.html
//
// 温馨提示：
// 此邮箱每天都收到很多垃圾邮件，邮件名请不要随意乱取，应尽量与垃圾邮件区分开来，幸好QQ的过滤功能还算强大~O(∩_∩)O~
// 此邮箱仅供【错误指正和技术交流】，若是技术问题请直接在文章评论区提问，否则恕不回复，这样做是为了让后面的小伙伴也能看到解决方案~O(∩_∩)O~
//
// 使用协议：WTFPL
// 鉴于在天朝神马协议都被无视，甚至很多大公司都不遵守，故索性采用了【DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE】协议~O(∩_∩)O~
//
// 代码安全性：
// 此项目为示例项目，为了方便大家编译，没用三方库，因此写了一些简单的函数，很多逻辑判断也只是保证正常操作，实际使用请自行保证代码安全~O(∩_∩)O~
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

	bool Play(const std::string &strPath);  // 播放路径为strPath的文件
	void Play();							// 播放
	void Pause();                           // 暂停
	void Stop();							// 停止

	void Volume(int iVol);                  // 音量设置为iVol

	void SeekTo(int iPos);                  // 跳到指定位置iPos
	void SeekForward();                     // 快进
	void SeekBackward();                    // 快退

	void SetHWND(HWND hwnd);                // 设置视频显示的窗口句柄
	HWND GetHWND();                         // 获取视频显示的窗口句柄

	bool    IsOpen();                       // 文件是否打开
	bool    IsPlaying();                    // 文件是否正在播放
	int     GetPos();                       // 获取文件当前播放的位置
	__int64 GetTotalTime();                 // 获取总时间
	__int64 GetTime();                      // 获取时间
	int     GetVolume();                    // 获取音量
	void ChangeWindowSize(RECT &rc);		// 修改当前播放视频的宽和高

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
	HWND video_hwnd_;						// 视频显示的窗口句柄
	char cmd_string_[40];
	BOOL is_allow_to_send_cmd_;
	// Trying to avoid flooded by Pipe crap
	BOOL is_pipe_full_;						// Pipe is full previously
	int pipe_full_count_;					// Number of times pipe is full
	HANDLE thread_send_cmd_string_;
	HANDLE send_cmd_string_, send_cmd_end_;
	int pipe_buffer_size_;

	void Init();                            // 初始化
	void Release();                         // 清理内存
	void RequireTotalTime();
	void RequireWidthHeight();

	int CreateCmdPipe();					// 创建命令管道
	void MakeupCmdLn();						// 生成命令行
	int DestroyCmdPipe();					// 销毁命令管道
	void CleanUp();							// cleanup something after playback
	void TerminateSendCmdThread();
	void GetCmdString(char[]);
	void SeekTime(DWORD dwSeekTime);		// 定位到一个绝对位置(单位秒).

	// thread related
	static DWORD WINAPI SendCmd(LPVOID p);
	void SendCmd();
	int Command(std::string strCommand);	// 通过管道为MPlayer发送命令
	void AddArgument(std::string strArgument);//添加参数
	int CheckActivate();					// 检查MPlayer进程是否正在运行

private:
	// 必要成员变量
	std::string cmd_param_;					// command line
	std::string play_exe_path_;				// MPlayer Binary file path name
	std::string file_path_;					// source filename, media clip filename

	int play_status_;						// current status, play, stop, pause, ...
	long video_time_length_;				// 影片总的长度
	long current_time_pos_;

	int video_width_;						// 影片的宽度
	int video_height_;						// 影片的高度

	int player_windows_width_;				// 播放窗口的宽度
	int player_windows_height_;				// 播放窗口的高度

	long last_time_pos_;					// 上次点击播放进度条的位置
	int last_volume_pos_;					// 上次点击时间进度条的位置
	int pause_seek_to_pos_;					// 记录暂停时候按下去的点
	//	bool is_chose_video_info_size_;		// 是否需要修改播放窗口，以适应影片

	bool is_already_obtain_video_info_;		// 是否已经得到了视频的宽和高
	bool is_already_change_windows_;		// 是否已经根据视频宽高修改对应的播放窗口

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

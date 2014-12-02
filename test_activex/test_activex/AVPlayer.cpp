#include "stdafx.h"

#include "strstream"
#include "fstream"
#include "iostream"
using namespace std;

#include "cmath"
#include "vector"

#include "AVPlayer.h"

#define PALYER_EXE_PATH "c:\\plugins_need\\ffplay.exe"


CAVPlayer::CAVPlayer(char *video_play_original_key) : 
	video_hwnd_(NULL),
	last_time_pos_(0),
	last_volume_pos_(50), // 初始化设置音量为50
	pause_seek_to_pos_(0),
	video_width_(0), 
	video_height_(0),
	player_windows_width_(0),
	player_windows_height_(0),
	//	is_chose_video_info_size_(false),
	read_from_child_(0),
	thread_send_cmd_string_(0),
	play_status_(MPLAYER_BEGIN),
	is_already_obtain_video_info_(false),
	is_already_change_windows_(false)
{
}


CAVPlayer::~CAVPlayer()
{
	Release();
}


void CAVPlayer::Init()
{
	//    if (! m_pVLC_Inst)
	{
		//        m_pVLC_Inst = libvlc_new(0, NULL);
	}
}


void CAVPlayer::Release()
{
	Stop();
//	if (m_nStatus == MPLAYER_STOPPING || m_nStatus == MPLAYER_STOPPED)
	{
		TerminateThread(process_infomation_.hThread, 0);
		TerminateProcess(process_infomation_.hProcess, 0);
		// ExitProcess(m_piProcessInfo.hProcess);

		DestroyCmdPipe();
		CleanUp();
	}
}


// Create the command pipe, to communite with MPlayer process
// Return values:
//  int : non-zero means sucess, zero means failure or already created
int CAVPlayer::CreateCmdPipe()
{
	if (read_from_child_ != NULL)
	{
		// already created
		return 0;
	}

	// create pipe

	// The SECURITY_ATTRIBUTES is used to create inheritable handles
	// the last member of the structure is set to TRUE so that the handle
	// will be inheritable.
	security_arrtibutes_.nLength = sizeof(SECURITY_ATTRIBUTES);
	security_arrtibutes_.lpSecurityDescriptor = NULL;
	security_arrtibutes_.bInheritHandle = TRUE;

	HANDLE hReadFromChild;
	// The CreatePipe operation creates a single unidirectional anonymous pipe,
	// and returns handles to its read and write ends. They are, by the
	// SECURITY_ATTRIBUTES, inheritable handles.
	if (!::CreatePipe(&hReadFromChild, &write_to_parent_, &security_arrtibutes_, pipe_buffer_size_))
	{
		// pipe failed
		return 0;
	}

	if (!::DuplicateHandle(::GetCurrentProcess(),
		hReadFromChild,
		::GetCurrentProcess(),
		&read_from_child_,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
	{
		// duplicate failed
		::CloseHandle(hReadFromChild);
		::CloseHandle(write_to_parent_);
		return 0;
	}
	::CloseHandle(hReadFromChild);

	HANDLE hWriteToChild;
	if (!::CreatePipe(&read_from_parent_, &hWriteToChild, &security_arrtibutes_, 0))
	{
		// pipe failed
		return 0;
	}

	if (!::DuplicateHandle(::GetCurrentProcess(),
		hWriteToChild,
		::GetCurrentProcess(),
		&write_to_child_,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
	{
		::CloseHandle(read_from_parent_);
		::CloseHandle(hWriteToChild);
		return 0;
	}
	::CloseHandle(hWriteToChild);

	return 1;
}


// Reset some member variables, called before start playing everytime
void CAVPlayer::Reset(void)
{
	strcpy_s(cmd_string_, "");
	is_allow_to_send_cmd_ = TRUE;

	// 	m_lTimePos = 0;
	// 	m_nVolume = 50;
	// 	isMute = false;
	// 	m_beforeMute = 0;

	is_pipe_full_ = FALSE;
	pipe_full_count_ = 0;
}


DWORD WINAPI CAVPlayer::SendCmd(LPVOID p)
{
	CAVPlayer* me=(CAVPlayer*)p;
	me->SendCmd();
	return 0;
}


void CAVPlayer::AddArgument(std::string strArgument)
{
	cmd_param_ += strArgument;
	cmd_param_ += " ";
}


// for SendCmd Thread to get member variable: m_chCmdString
void CAVPlayer::GetCmdString(char chCmdString[])
{
	strncpy_s(chCmdString,39, cmd_string_, 39);
	chCmdString[39] = '\x0';
}


void CAVPlayer::SendCmd()
{
	DWORD nWritten;
	char chCmdString[40];
	HANDLE hWriteToChild;

	while (TRUE)
	{
		if (WaitForSingleObject(send_cmd_string_, INFINITE) == WAIT_OBJECT_0)
		{
			hWriteToChild = write_to_child_;

			// terminate thread if the hWriteToChild is NULL
			//  which means MPlayer.exe process has already terminated, and pipe is closed
			if (hWriteToChild == NULL)
			{
				break;
			}

			GetCmdString(chCmdString);
			WriteFile(hWriteToChild, chCmdString, strlen(chCmdString), &nWritten, NULL);
			SetEvent(send_cmd_end_);
		}
		Sleep(10);
	}
}

// Start (Create) the SendCmdThread
//  return code:
//    int : 0, already created
//          1, success
//         -1, failed
int CAVPlayer::StartSendCmdThread(void)
{
	DWORD dwSendCmdid;
	if (thread_send_cmd_string_ == NULL)
	{
		// CreateEvent(lpEventAttributes, // null, gets a default security descriptor
		//             bManualReset, // TRUE, use ResetEvent() to set nonsiginaled
		//             bInitialState, // TRUE, initial state of the event object is signaled
		//                            // FALSE, initial state nonsignaled
		//             lpName, // name of the event object
		send_cmd_string_ = CreateEvent(NULL, FALSE, FALSE, NULL);
		send_cmd_end_ = CreateEvent(NULL, FALSE, FALSE, NULL);
		// m_hEventSendCmdQuit = CreateEvent(NULL, FALSE, FALSE, NULL);

		thread_send_cmd_string_ = CreateThread(NULL,0,SendCmd,
			this,
			CREATE_SUSPENDED,
			&dwSendCmdid);

		if (!thread_send_cmd_string_)
		{
			// thread failed to create, close events and return
			CloseHandle(send_cmd_string_);
			CloseHandle(send_cmd_end_);
			send_cmd_string_ = NULL;
			send_cmd_end_ = NULL;
			return -1;
		}

		ResumeThread(thread_send_cmd_string_);

		return 1;
	}
	return 0;
}


void CAVPlayer::MakeupCmdLn(void)
{
	play_exe_path_ = PALYER_EXE_PATH;
	std::string strTemp  = play_exe_path_;//"D:\\mplayer.exe"

	// Initialization
	cmd_param_ = "";

	// MPlayer Exe Pathname
	AddArgument(strTemp);

	// Video Panel Handler
	// TODO：可以通过这里修改视屏播放时候的大小尺寸
	ostrstream ss;
	// 	ss<<"-hwnd "<<(int)m_hVideoPanel<<" -x "<<m_nPlugWidth<<" -y "
	// 		<<(m_nPlugHeight - m_nControlPanelHeight)<<'\0';//ffplay
	//	ss << "-hwnd " << (int)m_hWnd << " -x " << 400 << " -y " << 300 << '\0';
	//	ss << "-hwnd " << (int)m_hWnd << '\0';

	if (player_windows_width_ != 0 && player_windows_height_ != 0)
	{
		ss << "-hwnd " << (int)video_hwnd_ << " -x " << player_windows_width_ 
			<< " -y " << player_windows_height_ << '\0';
	}
	else
	{
		ss << "-hwnd " << (int)video_hwnd_ << '\0';
	}

	AddArgument(ss.str());

	AddArgument("-volume 50");	// 一开始在这里设置了50的音量

	// These are must have config settings
	AddArgument("-slave 1");

	// Source (Clip to be play)
	AddArgument(file_path_);	//mplayer
	//	AddArgument("C:/HTML/1/2_files/T11.avi");
}


bool CAVPlayer::Play(const std::string &strPath)
{
	//	if (m_nStatus != MPLAYER_STOPPED) return FALSE;

	file_path_ = strPath;

	if (CreateCmdPipe() != 1) return FALSE;

	Reset();

	if (StartSendCmdThread() != 1) return FALSE;

	// MPlayer process return code
	int nReturnCode = 0;
	memset(&process_infomation_, 0, sizeof(process_infomation_));
	memset(&startupinfo_, 0, sizeof(startupinfo_));

	startupinfo_.cb = sizeof(startupinfo_);
	startupinfo_.hStdError = write_to_parent_;
	startupinfo_.hStdOutput = write_to_parent_;
	startupinfo_.hStdInput = read_from_parent_;
	startupinfo_.wShowWindow = SW_HIDE;
	startupinfo_.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	// Makeup Command Line
	MakeupCmdLn();

	//	LPTSTR cmd=_T(" D:\\video.mp4");
	// 	char ccmd[100];
	// 	sprintf(ccmd,"D:\\mplayer.exe D:\\video.mp4 -wid %d -slave",m_hVideoPanel);
	// 	showMsg(ccmd);
	// We need a writeable buffer for the command (silly Windows restriction)	
	//(LPTSTR)std::wstring(m_strCmdLn.begin(),m_strCmdLn.end()).c_str()
	//_bstr_t bstr=m_strCmdLn.c_str();(LPTSTR)bstr  m_strCmdLn.c_str()
	BOOL bStarted;

#ifdef _UNICODE
	bStarted = CreateProcess(NULL,(LPTSTR)(std::wstring(cmd_param_.begin(),cmd_param_.end()).c_str()),NULL,NULL,
		TRUE,CREATE_NO_WINDOW|DETACHED_PROCESS|CREATE_SUSPENDED,NULL,NULL,&startupinfo_,&process_infomation_);
#else
	bStarted = CreateProcess(NULL,(LPTSTR)cmd_param_.c_str(),NULL,NULL,
		TRUE,CREATE_NO_WINDOW|DETACHED_PROCESS|CREATE_SUSPENDED,NULL,NULL,&startupinfo_,&process_infomation_);
#endif

	if (bStarted)
	{
		// Now close the output pipes so we get true EOF/broken pipe
		::CloseHandle(write_to_parent_);
		::CloseHandle(read_from_parent_);

		// Set MPLAYER_STARTING state
		play_status_ = MPLAYER_STARTING;
		ResumeThread(process_infomation_.hThread);
		// Start timer, read MPlayer.exe output via pipe in OnTimer()
		//	SetTimer(m_hVideoPanel,TIMER_MPLAYER_STARTING,200,NULL);
		// Post a message to mainframe
		//	SendMessage(m_hVideoPanel,UWM_MPLAYER_STARTING,0,0);
	}
	else
	{
		// Failed to start
		DestroyCmdPipe();
		// Stop timer, terminate thread
		CleanUp();
		//	SendMessage(m_hVideoPanel,UWM_MPLAYER_FAILEDTOSTART,0,0);
	}

	RequireTotalTime();
	play_status_ = MPLAYER_PLAYING;

	// TODO：得到视频的高和宽 然后设置对应的变量
	RequireWidthHeight();

	// 已经从视频中得到了高和宽，那么需要修改对应的标志
	is_already_obtain_video_info_ = true;

	return TRUE;
}


void CAVPlayer::CleanUp(void)
{
	// Stop timers
	// 	KillTimer(m_hVideoPanel,TIMER_HALF_SECOND);
	// 	KillTimer(m_hVideoPanel,TIMER_ONE_SECOND);
	// 	// KillTimer(TIMER_FULLSCREENMOUSEHIDER);
	// 	KillTimer(m_hVideoPanel,TIMER_MPLAYER_STARTING);
	// KillTimer(TIMER_EIGHT_SECOND);
	TerminateSendCmdThread();
}


// Try to terminate the SendCmdThread
//  by repeatly SetEvent(m_hEventSendCmdStart), the SendCmdThread will realize the
//  hWriteToChild is NULL and terminate itself automaticlly
void CAVPlayer::TerminateSendCmdThread(void)
{
	if (thread_send_cmd_string_ != NULL)
	{
		DWORD dwExitCode;

		while (TRUE)
		{
			SetEvent(send_cmd_string_);
			::GetExitCodeThread(thread_send_cmd_string_, &dwExitCode);
			if (dwExitCode != STILL_ACTIVE)
			{
				break;
			}

			if (WaitForSingleObject(thread_send_cmd_string_, 100) == WAIT_OBJECT_0)
			{
				break;
			}
			Sleep(10);
		}

		CloseHandle(thread_send_cmd_string_);
		CloseHandle(send_cmd_string_);
		CloseHandle(send_cmd_end_);
		// CloseHandle(m_hEventSendCmdQuit);
		thread_send_cmd_string_ = NULL;
		send_cmd_string_ = NULL;
		send_cmd_end_ = NULL;
		// m_hEventSendCmdQuit = NULL;
	}
}

// Destroy the command pipe
// Return values:
//  int : -1 means no command pipe to be destroyed
//         0 means sucess
int CAVPlayer::DestroyCmdPipe()
{
	if (read_from_child_ == NULL)
	{
		return -1;
	}

	read_from_child_ = NULL;
	write_to_parent_ = NULL;
	write_to_child_ = NULL;
	read_from_parent_ = NULL;

	return 0;
}


void CAVPlayer::Stop()
{
	if ((play_status_ == MPLAYER_PLAYING) || (play_status_ == MPLAYER_PAUSED) || (play_status_ == MPLAYER_STARTING))
	{
		int nCounter = 0;

		// Check MPlayer quit message can avoid noise when TerminateProcess
		while (1)
		{
			Command("$q");
			Sleep(10);
			nCounter++;

			// ParseMPlayerOutput will set m_nStatus...
			if (play_status_ == MPLAYER_STOPPED)
			{
				break;
			}

			// Return if MPlayer is not running
			if (CheckActivate() != 0)//MPlayer仍在运行则返回0.
			{
				break;
			}

			// 50 for 5 seconds
			if (nCounter > 50)
			{
				// Waited too long, and nothing can be read, stop it
				break;
			}
		}

		TerminateThread(process_infomation_.hThread, 0);
		TerminateProcess(process_infomation_.hProcess, 0);
		// ExitProcess(m_piProcessInfo.hProcess);

		DestroyCmdPipe();
		CleanUp();

		//	SendMessage(m_hVideoPanel,UWM_MPLAYER_STOPPED, 0, 0);
		play_status_ = MPLAYER_STOPPED;
	}
}


void CAVPlayer::Play()
{
	//    if (m_pVLC_Player)
	{
		//        libvlc_media_player_play(m_pVLC_Player);
	}
}

void CAVPlayer::Pause()
{
	Command("$p");
	if (play_status_ == MPLAYER_PLAYING) 
		play_status_ = MPLAYER_PAUSED;
	else if(play_status_ == MPLAYER_PAUSED)
		play_status_ = MPLAYER_PLAYING;
}


// Check MPlayer process activate state
// Return values:
//  int :  0 MPlayer process is actived
//        -1 MPlayer process is terminated
int CAVPlayer::CheckActivate()
{
	DWORD nExitCode;

	if (GetExitCodeProcess(process_infomation_.hProcess, &nExitCode))
	{
		if (nExitCode == STILL_ACTIVE)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	return -2;
}


// Send command to MPlayer process by using pipe
// Parameters:
//  [in]  CString : command
// Return values:
//  int : 0 means successed
//        -1 means failed
int CAVPlayer::Command(std::string strCommand)
{
	if (write_to_child_ == NULL)
	{
		return -1;
	}

	// return if MPlayer is not running
	if (CheckActivate() != 0)
	{
		return -1;
	}

	strCommand = strCommand + "\n";

	if ((play_status_ == MPLAYER_PAUSED))
	{
		if ((strCommand.find("$p") != 0) && (strCommand.find("$q") != 0))
		{
			return -1; // For MPlayer 1 engine, don't send command while paused, unless going to resume it or stop it on purpose
		}
	}

	if (WaitForSingleObject(send_cmd_end_, 0) == WAIT_OBJECT_0)
	{
		is_allow_to_send_cmd_ = TRUE;
	}

	if (is_allow_to_send_cmd_)
	{
		is_allow_to_send_cmd_ = FALSE;

		// limit to 39 characters maximum
		strcpy_s(cmd_string_,strCommand.c_str());
		cmd_string_[39] = '\x0';
		SetEvent(send_cmd_string_);
	}

	return 0;
}


// TODO：声音部分等明天来了在处理------------------------------
void CAVPlayer::Volume(int iVol)
{
	std::string strVolume;
	int set_volume;

	if (iVol < 0)
	{
		return;
	}
	if (iVol == 0)
	{
		// 静音的情况
		set_volume = -last_volume_pos_;
		last_volume_pos_ = 0;
	}
	else
	{
		//	if ((m_nStatus == MPLAYER_PLAYING) || (m_nStatus == MPLAYER_PAUSED) || (m_nStatus == MPLAYER_STOPPED) || (m_nStatus == MPLAYER_STARTING))
		{
			set_volume = iVol - last_volume_pos_;
			last_volume_pos_ = iVol;
			// 		if ((m_nVolume + iVol < 0) || (m_nVolume + iVol > 100))
			// 		{
			// 			iVol = 100 - m_nVolume;
			// 		}

			//		m_nVolume += iVol; // save
			//		m_nVolume = iVol;
		}
	}

	//	if ((m_nStatus == MPLAYER_PLAYING) || (m_nStatus == MPLAYER_PAUSED))
	{
		ostrstream ss;
		ss<<"$v"<<(int)set_volume<<'\0';
		string command;
		command = ss.str();

		Command(std::string(ss.str()));
	}
}


int CAVPlayer::GetPos()
{
	//    if (m_pVLC_Player)
	{
		//        return (int)(1000 * libvlc_media_player_get_position(m_pVLC_Player));
	}

	return 0;
}


void CAVPlayer::SeekTo(int iPos)
{
	if (iPos < 0 || iPos > video_time_length_)
	{
		return;
	}

	if (play_status_ == MPLAYER_PLAYING)
	{
		// 首先需要获取当前的时候
		//		GetTime();
		int isktm = iPos - last_time_pos_;
		//		int isktm = (int)iPos;
		ostrstream strSeek;
		strSeek << "$s" << isktm << '\0';
		Command(std::string(strSeek.str()));

		ofstream fout;
		fout.open("c:\\test2.txt", ios::app);
		fout << "isktm : " << isktm << endl;
		fout << "dwSeekTime : " << iPos << endl;
		fout << "std::string(strSeek.str()) : " << std::string(strSeek.str()) << endl;
		fout << endl;
		fout.close();

		last_time_pos_ = iPos;
	}

	if(play_status_ == MPLAYER_PAUSED)
	{
		//	Pause();
		//	SeekTime(iPos);
		pause_seek_to_pos_ = iPos;
		//	Pause();
	}
	// 注释掉这个，否则播放大文件的时候，容易出现问题
	/*	Pause();Pause();*/
}


// 定位到一个时间点
// Parameters:
//  [in]  DWORD : 要定位到的时间点
void CAVPlayer::SeekTime(DWORD dwSeekTime)
{
	if (play_status_ == MPLAYER_PLAYING)
	{
		int isktm = (int)dwSeekTime;
		ostrstream strSeek;
		strSeek<<"$s"<<isktm<<'\0';
		Command(std::string(strSeek.str()));

		ofstream fout;
		fout.open("c:\\test2.txt", ios::app);
		fout << "isktm : " << isktm << endl;
		fout << "dwSeekTime : " << dwSeekTime << endl;
		fout << "std::string(strSeek.str()) : " << std::string(strSeek.str()) << endl;
		fout << endl;
		fout.close();
	}
	if(play_status_ == MPLAYER_PAUSED)
	{
		//	Pause();
		SeekTime(dwSeekTime);
		//	Pause();
	}
	// 注释掉这个，否则播放大文件的时候，容易出现问题
	/*	Pause();Pause();*/
}


void CAVPlayer::SeekForward()
{
	SeekTime(5);
}


void CAVPlayer::SeekBackward()
{
	SeekTime(-5);
}


void CAVPlayer::SetHWND( HWND hwnd )
{
	if (::IsWindow(hwnd))
	{
		video_hwnd_ = hwnd;
	}
}


HWND CAVPlayer::GetHWND()
{
	return video_hwnd_;
}


// TODO：应该在这里设置一个标志
bool CAVPlayer::IsOpen()
{
	//    return NULL != m_pVLC_Player;
	return true;
}


bool CAVPlayer::IsPlaying()
{
	//    if (m_pVLC_Player)
	{
		//        return (1 == libvlc_media_player_is_playing(m_pVLC_Player));
	}

	return false;
}


__int64 CAVPlayer::GetTotalTime()
{
	//   if (m_pVLC_Player)
	{
		//     return libvlc_media_player_get_length(m_pVLC_Player);
	}

	RequireTotalTime();

	return video_time_length_;
}


__int64 CAVPlayer::GetTime()
{
	//   if (m_pVLC_Player)
	{
		//        return libvlc_media_player_get_time(m_pVLC_Player);
	}

	DWORD lBytesRemain;
	DWORD nRead;
	DWORD i;
	char chTestBuffer[100], chBuffer[5];
	char* strMPlayerOutput = NULL;

	while (true)
	{
		Sleep(10);
		Command("$gc");//get_time_length
		memset(chTestBuffer, 0, 100);
		PeekNamedPipe(read_from_child_, chTestBuffer, 80, &lBytesRemain, 0, 0);

		strMPlayerOutput = new char[lBytesRemain];
		for (i = 0; i < lBytesRemain; i++)
		{
			chBuffer[0] = '\x0';
			ReadFile(read_from_child_, chBuffer, 1, &nRead, NULL);

			if ((chBuffer[0] == '\n') || (chBuffer[0] == '\r')) // newline, return
			{
				break;
			}
			strMPlayerOutput[i] = chBuffer[0];
		}

		if (strMPlayerOutput[0] == '$' && strMPlayerOutput[1] == 'c')
		{
			char* pstr = strMPlayerOutput + 2;
			std::string slen = pstr;
			int t = 0;
			//	while((*pstr <= '9' && *pstr >= '0')||*pstr == '.'){ ++pstr; ++t; }
			current_time_pos_ = atol(slen.c_str());

			if (strMPlayerOutput)
			{
				delete strMPlayerOutput;
				strMPlayerOutput = NULL;
			}

			break;
		}
	}

	return current_time_pos_;
}


int CAVPlayer::GetVolume()
{
	//   if (m_pVLC_Player)
	{
		//        return libvlc_audio_get_volume(m_pVLC_Player);
	}

	return 0;
}


// 得到影片的时间总长度
void CAVPlayer::RequireTotalTime()
{
	if (CheckActivate() != 0)
	{
		return;
	}

	DWORD lBytesRemain;
	DWORD nRead;
	DWORD i;
	char chTestBuffer[100], chBuffer[5];
	char* strMPlayerOutput = NULL;

	while (true)
	{
		Sleep(10);
		Command("$gt");//get_time_length
		PeekNamedPipe(read_from_child_, chTestBuffer, 80, &lBytesRemain, 0, 0);

		if (lBytesRemain > 80)
		{
			lBytesRemain = 80;
		}
		strMPlayerOutput = new char[lBytesRemain];
		for (i = 0; i < lBytesRemain; i++)
		{
			chBuffer[0] = '\x0';
			ReadFile(read_from_child_, chBuffer, 1, &nRead, NULL);

			if ((chBuffer[0] == '\n') || (chBuffer[0] == '\r')) // newline, return
			{
				break;
			}
			strMPlayerOutput[i] = chBuffer[0];
		}

		if (strMPlayerOutput[0] == '$' && strMPlayerOutput[1] == 't')
		{
			char* pstr = strMPlayerOutput + 2;
			std::string slen = pstr;
			video_time_length_ = atol(slen.c_str());

			if (strMPlayerOutput)
			{
				delete strMPlayerOutput;
				strMPlayerOutput = NULL;
			}

			break;
		}
	}
}


void CAVPlayer::RequireWidthHeight()
{
	if (CheckActivate() != 0)
	{
		return;
	}

	DWORD lBytesRemain;
	DWORD nRead;
	DWORD i;
	char chTestBuffer[100], chBuffer[5];
	char* strMPlayerOutput = NULL;

	while (true)
	{
		Sleep(10);
		Command("$gw");//get_time_length
		PeekNamedPipe(read_from_child_, chTestBuffer, 80, &lBytesRemain, 0, 0);

		strMPlayerOutput = new char[lBytesRemain];
		for (i = 0; i < lBytesRemain; i++)
		{
			chBuffer[0] = '\x0';
			ReadFile(read_from_child_, chBuffer, 1, &nRead, NULL);

			if ((chBuffer[0] == '\n') || (chBuffer[0] == '\r')) // newline, return
			{
				break;
			}
			strMPlayerOutput[i] = chBuffer[0];
		}

		// 		if (strMPlayerOutput[0] == '$' && strMPlayerOutput[1] == 't')
		// 		{
		// 			char* pstr = strMPlayerOutput + 2;
		// 			std::string slen = pstr;
		// 			video_time_length_ = atol(slen.c_str());
		// 
		// 			if (strMPlayerOutput)
		// 			{
		// 				delete strMPlayerOutput;
		// 			}
		// 
		// 			break;
		// 		}

		if (strMPlayerOutput[0] == '$' && strMPlayerOutput[1] == 'w')
		{
			int w = 0, h = 0;
			char* pstr = strMPlayerOutput + 2;
			while(*pstr <= '9' && *pstr >= '0')
			{
				w = 10 * w + *pstr - '0';
				++ pstr;
			}
			++ pstr;
			while(*pstr <= '9' && *pstr >= '0')
			{
				h = 10 * h + *pstr - '0';
				++ pstr;
			}
			video_width_ = w;
			video_height_ = h;

			if (strMPlayerOutput)
			{
				delete strMPlayerOutput;
				strMPlayerOutput = NULL;
			}

			break;
		}
	}
}


void CAVPlayer::ChangeWindowSize(RECT &rc)
{
	ostrstream ss;
	// 	ss<<"-hwnd "<<(int)m_hVideoPanel<<" -x "<<m_nPlugWidth<<" -y "
	// 		<<(m_nPlugHeight - m_nControlPanelHeight)<<'\0';//ffplay
	//	ss << "-hwnd " << (int)m_hWnd << " -x " << 400 << " -y " << 300 << '\0';
	ss << "-hwnd " << (int)video_hwnd_ << " -x " << 100 << " -y " << 100 << '\0';

	AddArgument(ss.str());
}
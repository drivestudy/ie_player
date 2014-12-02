#ifndef  __PLAY_LIST_H__
#define  __PLAY_LIST_H__


#include "iostream"
#include "string"
#include "vector"
#include "unordered_map"
using namespace std;


class PlayList
{
public:
	PlayList();
	~PlayList();

	bool ReadFile(const string &path);
	bool WriteFile(const string &path);

	void AddPlayVideoPath(string play_video_path);

private:
	bool ReadFile(const string &path,  vector<string> &vec);
	bool WriteFile(const string &path, vector<string> &vec);

private:
	vector<string> all_video_path_vec_;
	unordered_map<string, string> all_video_path_map_;
};


#endif // end if __PLAY_LIST_H__
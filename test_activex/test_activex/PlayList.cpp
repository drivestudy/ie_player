#include "stdafx.h"
#include "fstream"
#include "PlayList.h"


PlayList::PlayList()
{

}


PlayList::~PlayList()
{

}


bool PlayList::ReadFile(const string &path)
{
	return ReadFile(path, all_video_path_vec_);
}


bool PlayList::WriteFile(const string &path)
{
	return WriteFile(path, all_video_path_vec_);
}


bool PlayList::ReadFile(const string &path, vector<string> &vec)
{
	ifstream fin;
	fin.open(path);
	if (!fin.is_open())
	{
		return false;
	}

	// 取出数据
	string tmp;
	while (fin >> tmp)
	{
		cout << tmp << endl;
		//		vec.push_back(tmp);
		AddPlayVideoPath(tmp);
	}

	fin.close();

	return true;
}


bool PlayList::WriteFile(const string &path, vector<string> &vec)
{
	ofstream fout;
	fout.open(path);

	vector<string>::iterator it;
	for (it = vec.begin(); it != vec.end(); it++)
	{
		fout << *it << endl;
	}
	fout.close();

	return true;
}


void PlayList::AddPlayVideoPath(string play_video_path)
{
	unordered_map<string, string>::iterator it;
	it = all_video_path_map_.find(play_video_path);
	if (it != all_video_path_map_.end())
	{
		return;
	}

	all_video_path_map_.insert(pair<string, string>(play_video_path, ""));
	all_video_path_vec_.push_back(play_video_path);
}

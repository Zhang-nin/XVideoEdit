#pragma once
#include<string>
#include<qmutex.h>
class XAudio
{
public:
	QMutex mutex;
	static XAudio* Get();

	//导出音频文件
	virtual bool ExportA(std::string src, std::string out, int beginMs, int endMs) = 0;
	//合并音频文件
	virtual bool Merge(std::string v, std::string a, std::string out) = 0;

	XAudio();
	virtual ~XAudio();
};


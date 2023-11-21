#pragma once
#include<string>
#include<qmutex.h>
class XAudio
{
public:
	QMutex mutex;
	static XAudio* Get();

	//������Ƶ�ļ�
	virtual bool ExportA(std::string src, std::string out, int beginMs, int endMs) = 0;
	//�ϲ���Ƶ�ļ�
	virtual bool Merge(std::string v, std::string a, std::string out) = 0;

	XAudio();
	virtual ~XAudio();
};


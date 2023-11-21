#include "XAudio.h"
#include<iostream>
using namespace std;
struct XTime
{
	XTime(int tms) {
		h = (tms / 1000) / 3600;
		m = ((tms / 1000) % 3600) / 60;
		s = (tms / 1000) % (3600 * 60);
		ms = tms % 1000;
	}
	std::string ToString() {
		char buf[16] = {};
		sprintf(buf, "%d:%d:%d.%d", h, m, s, ms);
		return buf;
	}
	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;
};
class CXAudio : public XAudio {
public:
	bool ExportA(std::string src, std::string out, int beginMs = 0, int endMs = 0) {
		//ffmpeg -i 1.mkv -ss 0:1:10.112 -t 0:1:10.100 -vn 1.mp3
		string cmd = "ffmpeg ";
		cmd += "-i ";
		cmd += src;
		if (beginMs > 0) {
			cmd += " -ss ";
			XTime xt(beginMs);
			cmd += xt.ToString();
		}
		if (endMs > 0) {
			cmd += " -t ";
			XTime xt(endMs);
			cmd += xt.ToString();
		}
		cmd += " -vn -y ";
		cmd += out;
		cout << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}
	//合并音频文件
	virtual bool Merge(std::string v, std::string a, std::string out) {
		//ffmpeg -i 1.mp3 -i 1.mp4 -c copy out.mp4
		string cmd = "ffmpeg -i ";
		cmd += v;
		cmd += " -i ";
		cmd += a;
		cmd += " -c copy ";
		cmd += out;
		cout << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}
};
XAudio* XAudio::Get() {
	static CXAudio ca;
	return &ca;
}
XAudio::XAudio() {

}

XAudio::~XAudio() {

}


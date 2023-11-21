#include "XVideoThread.h"
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include"XFilter.h"
using namespace cv;
using namespace std;
//一号视频源
static VideoCapture cap1;

//保存视频
static VideoWriter vw;

static bool isexit = false;
//打开一号视频源
bool XVideoThread::Open(const std::string file) {
	cout << "open:" << file << endl;
	mutex.lock();
	bool re = cap1.open(file);
	mutex.unlock();
	cout << re << endl;
	if(!re)
		return re;
	fps = cap1.get(CAP_PROP_FPS);
	width = cap1.get(CAP_PROP_FRAME_WIDTH);
	height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0) fps = 25;
	return true;
}

//考虑帧不相同
bool XVideoThread::Seek(int frame) {
	mutex.lock();
	if (!cap1.isOpened()) {
		mutex.unlock();
		return false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
	mutex.unlock();
	return re;
}

bool XVideoThread::Seek(double pos) {
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	return Seek(frame);
}

void XVideoThread::run() {
	Mat mat1;
	for (;;) {
		mutex.lock();
		if (isexit) {
			mutex.unlock();
			break;
		}
		//判断视频是否打开
		if (!cap1.isOpened()) {
			mutex.unlock();
			msleep(5);
			continue;
		}
		SetEnable();
		//暂停
		if (!isPlay) {
			mutex.unlock();
			msleep(5);
			continue;
		}
		//读取一帧视频，解码并做颜色转换
		if (!cap1.read(mat1) || mat1.empty()) {
			mutex.unlock();
			//导出到结尾位置停止导出
			if (isWrite) {
				StopSave();
				SaveEnd();
			}
				
			msleep(5);
			continue;
		}
		//显示图像1
		if(!isWrite)
			ViewImage1(mat1);
		Mat mat2 = mark;
		Mat des = XFilter::Get()->Filter(mat1, mat2);
		//显示图像2
		if (!isWrite)
			ViewDes(des);

		int s = 0;
		s = 1000 / fps;

		//写入
		if (isWrite) {
			s = 1;
			vw.write(des);
		}

		msleep(s);
		mutex.unlock();
	}
}

double XVideoThread::GetPos() {
	double pos = 0;
	mutex.lock();
	if (!cap1.isOpened()) {
		mutex.unlock();
		return pos;
	}
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	double cur = cap1.get(CAP_PROP_POS_FRAMES);
	if (count > 0.001)
		pos = cur / count;
	mutex.unlock();
	return pos;
}

bool XVideoThread::StartSave(const std::string filename, int width, int height, bool isColor) {
	Seek(0);
	cout << "开始导出" << endl;
	mutex.lock();
	if (!cap1.isOpened()) {
		mutex.unlock();
		return false;
	}
	if (width <= 0) width = cap1.get(CAP_PROP_FRAME_WIDTH);
	if (height <= 0) height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	vw.open(filename,
		VideoWriter::fourcc('X', '2', '6', '4'),
		this->fps,
		Size(width, height),
		isColor
	);
	if (!vw.isOpened()) {
		mutex.unlock();
		cout << "star save failed" << endl;
		return false;
	}
	this->isWrite = true;
	mutex.unlock();
	return true;
}

void XVideoThread::StopSave() {
	cout << "停止导出" << endl;
	mutex.lock();
	vw.release();
	isWrite = false;
	mutex.unlock();
}

XVideoThread::XVideoThread() {
	start();
}

XVideoThread::~XVideoThread() {
	mutex.lock();
	isexit = true;
	mutex.unlock();
	wait();
}


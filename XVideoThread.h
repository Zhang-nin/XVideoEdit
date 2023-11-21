#pragma once
#include<qthread.h>
#include<qmutex.h>
#include<opencv2/core.hpp>
class XVideoThread : public QThread
{
	Q_OBJECT
public:
	int fps = 0;
	int width = 0;
	int height = 0;
	//����ģʽ ��ȡ����
	static XVideoThread* Get() {
		static XVideoThread vt;
		return &vt;
	}
	bool Open(const std::string file);
	void Play() { mutex.lock(); isPlay = true; mutex.unlock(); };
	void Pause() { mutex.lock(); isPlay = false; mutex.unlock(); };
	//���ص�ǰ����λ��
	double GetPos();

	//��ת��Ƶ
	bool Seek(int frame);
	bool Seek(double pos);

	//��ʼ������Ƶ
	bool StartSave(const std::string filename, int width = 0, int height = 0);

	//ֹͣ������Ƶ��д����Ƶ֡����
	void StopSave();

	~XVideoThread();

	//�߳���ں���
	void run();

signals:
	//��ʾԭ��Ƶͼ��
	void ViewImage1(cv::Mat mat);
	//��ʾ���ɺ�ͼ��
	void ViewDes(cv::Mat mat);
	void SaveEnd();
	void SetEnable();
protected:
	QMutex mutex;
	bool isWrite = false;
	bool isPlay = false;
	XVideoThread();
};


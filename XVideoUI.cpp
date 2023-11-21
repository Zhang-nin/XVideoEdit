#include "XVideoUI.h"
#include<qfiledialog.h>
#include<qmessagebox.h>
#include<string>
#include<iostream>
#include"XVideoThread.h"
#include"XFilter.h"
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
using namespace std;

static bool pressSlider = false;
static bool isExport = false;
static bool isColor = true;
static bool isMark = false;
static bool isBlend = false;
static bool isMerge = false;
XVideoUI::XVideoUI(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlag(Qt::FramelessWindowHint);
	qRegisterMetaType<cv::Mat>("cv::Mat");

	//ԭ��Ƶ�ź�
	QObject::connect(
		XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat))
	);
	//�����Ƶ�ź�
	QObject::connect(
		XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat))
	);
	//������Ƶ����
	QObject::connect(
		XVideoThread::Get(),
		SIGNAL(SaveEnd()),
		this,
		SLOT(ExportEnd())
	);
	//����Ƶ��ť����
	QObject::connect(
		XVideoThread::Get(),
		SIGNAL(SetEnable()),
		this,
		SLOT(ButSetEnable())
	);

	//����ں���Ƶ
	QObject::connect(
		XVideoThread::Get(),
		SIGNAL(ViewImage2(cv::Mat)),
		ui.src2,
		SLOT(SetImage(cv::Mat))
	);

	Pause();
	ButSetEnable(false);
	startTimer(40);

}

void XVideoUI::timerEvent(QTimerEvent* e) {
	if (pressSlider) return;
	double pos = XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos * 1000);
}

void XVideoUI::Open() {
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ѡ����Ƶ�ļ�"));
	if (name.isEmpty()) return;
	string file = name.toLocal8Bit().data();

	if (!XVideoThread::Get()->Open(file)) {
		QMessageBox::information(this, "error", name + " open failed!");
		return;
	}
	//QMessageBox::information(this, "", name);
	ui.width->setValue(XVideoThread::Get()->width);
	ui.height->setValue(XVideoThread::Get()->height);
	Play();
}

void XVideoUI::SliderPress() {
	pressSlider = true;
}
void XVideoUI::SliderRelease() {
	pressSlider = false;
}
//�������϶�
void XVideoUI::SetPos(int pos) {
	XVideoThread::Get()->Seek((double)pos / 1000.);
}

void XVideoUI::Set() {
	XFilter::Get()->Clear();
	//��Ƶͼ��ü�
	bool isClip = false;
	double cx = ui.cx->value();
	double cy = ui.cy->value();
	double cw = ui.cw->value();
	double ch = ui.ch->value();

	if (cx + cy + cy + ch > 0.0001) {
		isClip = true;
		XFilter::Get()->Add(XTask{ XTASK_CLIP,
			{cx, cy, cw, ch}
			});
		double w = XVideoThread::Get()->width;
		double h = XVideoThread::Get()->height;
		XFilter::Get()->Add(XTask{ XTASK_RESIZE,
			{w, h}
			});
	}

	//ͼ�������
	bool ispy = false;
	double down = ui.pydown->value();
	double up = ui.pyup->value();
	if (!isClip && down > 0.0001) {
		ispy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYDOWN,
			{down}
			});
		int w = XVideoThread::Get()->width;
		int h = XVideoThread::Get()->height;
		for (int i = 0; i < down; i++)
		{
			w /= 2;
			h /= 2;
		}
		ui.width->setValue(w);
		ui.height->setValue(h);
	}
	if (!isClip && up > 0.0001) {
		ispy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYUP,
			{up}
			});
		int w = XVideoThread::Get()->width;
		int h = XVideoThread::Get()->height;
		for (int i = 0; i < up; i++)
		{
			w *= 2;
			h *= 2;
		}
		ui.width->setValue(w);
		ui.height->setValue(h);
	}
	//������Ƶ�ߴ�
	double w = ui.width->value();
	double h = ui.height->value();
	if (!isMerge && !isClip && !ispy && w > 0 && h > 0) {
		if (w != XVideoThread::Get()->width || h != XVideoThread::Get()->height) 
			XFilter::Get()->Add(XTask{ XTASK_RESIZE,
				{w, h}
				});
	}

	//�Աȶ�����
	if (ui.bright->value() > 0 || ui.contrast->value() > 1) {
		XFilter::Get()->Add(XTask{ XTASK_GAIN,
			{(double)ui.bright->value(), ui.contrast->value()}
			});
	}

	//�Ҷ�ͼ
	switch (ui.color->currentIndex())
	{
	case 0:
		isColor = true;
		break;
	case 1:
		XFilter::Get()->Add(XTask{ XTASK_GRAY });
		isColor = false;
		break;
	default:
		break;
	}

	//ͼ����ת
	switch (ui.rotate->currentIndex())
	{
	case 1:
		XFilter::Get()->Add(XTask{ XTASK_RORATE90 });
		break;
	case 2:
		XFilter::Get()->Add(XTask{ XTASK_RORATE180 });
		break;
	case 3:
		XFilter::Get()->Add(XTask{ XTASK_RORATE270 });
		break;
	default:
		break;
	}

	//ͼ����
	switch (ui.flip->currentIndex())
	{
	case 1:
		XFilter::Get()->Add(XTask{ XTASK_FLIPX });
		break;
	case 2:
		XFilter::Get()->Add(XTask{ XTASK_FLIPY });
		break;
	case 3:
		XFilter::Get()->Add(XTask{ XTASK_FLIPXY });
		break;
	default:
		break;
	}

	//ˮӡ
	if (isMark) {
		double x = ui.mx->value();
		double y = ui.my->value();
		double a = ui.ma->value();
		XFilter::Get()->Add(XTask{ XTASK_MASK,
			{x, y, a}
			});
	}
	//�ں�
	if (isBlend) {
		double a = ui.ba->value();
		XFilter::Get()->Add(XTask{XTASK_BLEND,{a}});
	}
	//�ϲ�
	if (isMerge) {
		XFilter::Get()->Add(XTask{ XTASK_MERGE});
		int w1 = XVideoThread::Get()->width;
		int w2 = XVideoThread::Get()->width2;
		int h1 = XVideoThread::Get()->height;
		int h2 = XVideoThread::Get()->height2;
		if (h1 != h2)
			w2 *= (double)h2 / (double)h1;
		ui.width->setValue(w1+(int)w2);
		ui.height->setValue(h1);
	}
}

//������Ƶ
void XVideoUI::Export() {
	if (isExport) {
		XVideoThread::Get()->StopSave();
		isExport = false;
		ui.exportButton->setText("Start Export");
		return;
	}
	QString name = QFileDialog::getSaveFileName(this, "save", "out1.avi");
	if (name.isEmpty()) return;
	std::string file = name.toLocal8Bit().data();
	int w = ui.width->value();
	int h = ui.height->value();
	if (XVideoThread::Get()->StartSave(file, w, h, isColor)) {
		isExport = true;
		ui.exportButton->setText("Stop Export");
	}
}

//��������
void XVideoUI::ExportEnd() {
	isExport = false;
	ui.exportButton->setText("Start Export");
}

//����
void XVideoUI::Play() {
	ui.pauseButton->show();
	ui.pauseButton->setGeometry(ui.playButton->geometry());
	ui.playButton->hide();
	XVideoThread::Get()->Play();
}
//��ͣ
void XVideoUI::Pause() {
	ui.playButton->show();
	//ui.pauseButton->setGeometry(ui.playButton->geometry());
	ui.pauseButton->hide();
	XVideoThread::Get()->Pause();
}

//ˮӡ
void XVideoUI::Mark() {
	isMark = false;
	isBlend = false;
	isMerge = false;
	QString name = QFileDialog::getOpenFileName(this, "select image:");
	if (name.isEmpty()) {
		return;
	}
	std::string file = name.toLocal8Bit().data();
	cv::Mat mark = cv::imread(file);
	if (mark.empty())return;
	XVideoThread::Get()->SetMark(mark);
	isMark = true;
}

//�ں�
void XVideoUI::Blend() {
	isMark = false;
	isBlend = false;
	isMerge = false;
	QString name = QFileDialog::getOpenFileName(this, "select video:");
	if (name.isEmpty()) {
		return;
	}
	std::string file = name.toLocal8Bit().data();
	isBlend = XVideoThread::Get()->Open2(file);
}

//�ϲ�
void XVideoUI::Merge() {
	isMark = false;
	isBlend = false;
	isMerge = false;
	QString name = QFileDialog::getOpenFileName(this, "select video:");
	if (name.isEmpty()) {
		return;
	}
	std::string file = name.toLocal8Bit().data();
	isMerge = XVideoThread::Get()->Open2(file);
}

void XVideoUI::ButSetEnable(bool flag) {
	ui.playButton->setEnabled(flag);
	ui.exportButton->setEnabled(flag);
	ui.setButton->setEnabled(flag);
}
#include "XImagePro.h"
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
//设置原图，会清理处理结果
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2) {
	if (mat1.empty()) return;
	this->src1 = mat1;
	this->src2 = mat2;
	this->src1.copyTo(des);
}

//设置亮度对比度
//bright 亮度0~100
//contrast 对比度 1.0~3.0
void XImagePro::Gain(double bright, double contrast) {
	if (des.empty())return;
	des.convertTo(des, -1, contrast, bright);
}
void XImagePro::Rotate90() {
	if (des.empty())return;
	rotate(des, des, ROTATE_90_CLOCKWISE);
}
void XImagePro::Rotate180() {
	if (des.empty())return;
	rotate(des, des, ROTATE_180);
}
void XImagePro::Rotate270() {
	if (des.empty())return;
	rotate(des, des, ROTATE_90_COUNTERCLOCKWISE);
}

void XImagePro::FilpX() {
	if (des.empty())return;
	flip(des, des, 0);
}
void XImagePro::FilpY() {
	if (des.empty())return;
	flip(des, des, 1);
}
void XImagePro::FilpXY() {
	if (des.empty())return;
	flip(des, des, -1);
}

void XImagePro::Resize(int width, int height) {
	if (des.empty())return;
	resize(des, des, Size(width, height));
}

//图像金字塔
void XImagePro::PyDown(int count) {
	if (des.empty())return;
	for (int i = 0; i < count; i++)
	{
		pyrDown(des, des);
	}
}
void XImagePro::PyUp(int count) {
	if (des.empty())return;
	for (int i = 0; i < count; i++)
	{
		pyrUp(des, des);
	}
}

void XImagePro::Clip(int x, int y, int w, int h) {
	if (des.empty())return;
	if (x < 0 || y < 0 || w <= 0 || h <= 0) return;
	if (x > des.cols || y > des.rows) return;
	des = des(Rect(x, y, w, h));
}

void XImagePro::Gray() {
	if (des.empty())return;
	cvtColor(des, des, COLOR_BGR2GRAY);
}

//水印
void XImagePro::Mark(int x, int y, double a) {
	if (des.empty()) return;
	if (src2.empty()) return;
	Mat rol = des(Rect(x, y, src2.cols, src2.rows));
	addWeighted(src2, a, rol, 1 - a, 0, rol);
}

void XImagePro::Blend(double a) {
	if (des.empty()) return;
	if (src2.empty()) return;
	if (src2.size() != des.size())
		resize(src2, src2, des.size());
	addWeighted(src2, a, des, 1 - a, 0, des);
}

void XImagePro::Merge() {
	if (des.empty()) return;
	if (src2.empty()) return;
	if (src2.rows != des.rows) {
		int w = src2.cols * ((double)src2.rows / (double)des.rows);
		resize(src2, src2, Size(w,des.rows));
	}
	int dw = src1.cols + src2.cols;
	int dh = src1.rows;
	des = Mat(Size(dw, dh), src1.type());
	Mat r1 = des(Rect(0, 0, src1.cols, dh));
	Mat r2 = des(Rect(src1.cols, 0, src2.cols, dh));
	src1.copyTo(r1);
	src2.copyTo(r2);
}

XImagePro::XImagePro(){

}
XImagePro::~XImagePro() {

}

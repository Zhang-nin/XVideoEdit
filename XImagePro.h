#pragma once
#include<opencv2/core.hpp>
class XImagePro
{
public:
	//设置原图，会清理处理结果
	void Set(cv::Mat mat1, cv::Mat mat2);

	//获取处理后结果
	cv::Mat Get() {
		return des;
	}
	//设置亮度对比度
	//bright 亮度0~100
	//contrast 对比度 1.0~3.0
	void Gain(double bright, double contrast);

	void Rotate90();
	void Rotate180();
	void Rotate270();

	void FilpX();
	void FilpY();
	void FilpXY();

	void Resize(int width, int height);

	//图像金字塔
	void PyDown(int count);
	void PyUp(int count);

	//视频画面裁剪
	void Clip(int x, int y, int w, int h);

	XImagePro();
	~XImagePro();
private:
	//原图
	cv::Mat src1, src2;
	//目标图
	cv::Mat des;
};


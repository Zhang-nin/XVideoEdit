#pragma once
#include<opencv2/core.hpp>
class XImagePro
{
public:
	//����ԭͼ������������
	void Set(cv::Mat mat1, cv::Mat mat2);

	//��ȡ�������
	cv::Mat Get() {
		return des;
	}
	//�������ȶԱȶ�
	//bright ����0~100
	//contrast �Աȶ� 1.0~3.0
	void Gain(double bright, double contrast);

	void Rotate90();
	void Rotate180();
	void Rotate270();

	void FilpX();
	void FilpY();
	void FilpXY();

	void Resize(int width, int height);

	//ͼ�������
	void PyDown(int count);
	void PyUp(int count);

	//��Ƶ����ü�
	void Clip(int x, int y, int w, int h);

	XImagePro();
	~XImagePro();
private:
	//ԭͼ
	cv::Mat src1, src2;
	//Ŀ��ͼ
	cv::Mat des;
};


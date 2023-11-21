#pragma once
#include<opencv2/core.hpp>

enum XTaskType {
	XTASK_NONE,
	XTASK_GAIN, //亮度对比度调整
	XTASK_RORATE90,
	XTASK_RORATE180,
	XTASK_RORATE270,
	XTASK_FLIPX,
	XTASK_FLIPY,
	XTASK_FLIPXY,
	XTASK_RESIZE,
	XTASK_PYDOWN,
	XTASK_PYUP,
	XTASK_CLIP,
	XTASK_GRAY,
	XTASK_MASK
};
struct XTask
{
	XTaskType type;
	std::vector<double> para;
};
class XFilter
{
public:
	virtual cv::Mat Filter(cv::Mat mat1, cv::Mat mat2)=0;
	virtual void Add(XTask task) = 0;
	virtual void Clear() = 0;
	static XFilter* Get();
	virtual ~XFilter();
protected:
	XFilter();
};


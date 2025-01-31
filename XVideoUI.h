#pragma once

#include <QtWidgets/QWidget>
#include "ui_XVideoUI.h"

class XVideoUI : public QWidget
{
    Q_OBJECT

public:
    XVideoUI(QWidget *parent = Q_NULLPTR);
    void timerEvent(QTimerEvent *e);
public slots:
    void Open();
    void SliderPress();
    void SliderRelease();
    //滑动条拖动
    void SetPos(int);
    //设置过滤器
    void Set();
    //导出视频
    void Export();
    //导出结束
    void ExportEnd();
    //播放
    void Play();
    //暂停
    void Pause();
    //启用按钮
    void ButSetEnable(bool flag=true);
    //水印
    void Mark();
    //融合
    void Blend();
    //合并
    void Merge();

    void Left(int pos);
    void Right(int pos);
private:
    Ui::XVideoUIClass ui;
};

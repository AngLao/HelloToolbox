#ifndef __COMBOBOX_H__
#define __COMBOBOX_H__

#pragma once

#include "qevent.h"
#include <QComboBox>

class MyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyComboBox(QWidget *parent = nullptr){};
    ~MyComboBox(){};
protected:
    //添加鼠标点击事件
    virtual void PressEvent(QMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
            emit clicked();  //触发clicked信号

        QComboBox::mousePressEvent(event);  //将该事件传给父类处理，这句话很重要，如果没有，父类无法处理本来的点击事件
    }

signals:
    void clicked();  //自定义点击信号，在mousePressEvent事件发生时触发
};

#endif

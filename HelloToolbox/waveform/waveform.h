#ifndef __WAVEFORM_H__
#define __WAVEFORM_H__

#include <QObject>
#include "ui_Waveform.h"

#include "qcustomplot.h"

class WaveformDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformDisplay(void);
    ~WaveformDisplay(){};

    QWidget* Widget(){
        return &widget;
    };

    void paintGyroData(float gyroX, float gyroY, float gyroZ);
private:
    void lineInit();
    unsigned long frameCount = 0;

private:
    Ui_Form ui;
    QWidget widget;

    QCustomPlot qcustomplot;
    //按钮标志位
    bool startFlag = false;
    bool dynamicDisplayFlag =true;
};

#endif // WAVEFORMDISPLAY_H

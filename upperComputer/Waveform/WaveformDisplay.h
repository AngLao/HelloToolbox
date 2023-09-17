
#ifndef WAVEFORMDISPLAY_H
#define WAVEFORMDISPLAY_H


#include "qcustomplot.h"
#include <QObject>


class WaveformDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformDisplay(void);
    QStringList userColorStrList;
    QCustomPlot* pQCustomPlot;
    //按钮标志位
    bool startFlag = false;
    bool dynamicDisplayFlag =true;

    QWidget* Widget(){
        return pWidget;
    };
    void ProcessingMessages(uint8_t*pData ,uint8_t len);
private:
    QWidget* pWidget;
    void paintUserData(double x ,double y , int num);
    void lineInit();
signals:

};

#endif // WAVEFORMDISPLAY_H

#include "waveform.h"


WaveformDisplay::WaveformDisplay(void)
{
    ui.setupUi(&widget);
    ui.widgetLayout->addWidget(&qcustomplot);

    //开始显示按钮
    connect(ui.startWaveformButton,&QPushButton::clicked,this,[=](){
        if(ui.startWaveformButton->text() == "开始显示"){
            ui.startWaveformButton->setText("暂停显示");
            startFlag = true;
        }else{
            ui.startWaveformButton->setText("开始显示");
            startFlag = false;
        }
    });
    //清空波形按钮
    connect(ui.clearWaveformButton,&QPushButton::clicked,this,[=](){
        int count = qcustomplot.graphCount();//获取曲线条数
        for(int i = 0; i < count; ++i){
            qcustomplot.graph(i)->data().data()->clear();
        }
        qcustomplot.replot();

        frameCount = 0;
    });
    //动态显示按钮
    connect(ui.dynamicDisplayButton,&QPushButton::clicked,this,[=](){
        if(ui.dynamicDisplayButton->text() == "动态显示"){
            ui.dynamicDisplayButton->setText("停止跟随");
            dynamicDisplayFlag = true;
        }else{
            ui.dynamicDisplayButton->setText("动态显示");
            dynamicDisplayFlag = false;
        }
    });
    //波形全显按钮
    connect(ui.seeAllLineButton,&QPushButton::clicked,this,[=](){
        ui.dynamicDisplayButton->setText("动态显示");
        dynamicDisplayFlag = false;
        qcustomplot.xAxis->rescale(true);//调整X轴的范围，使之能显示出所有的曲线的X值
        qcustomplot.yAxis->rescale(true);//调整Y轴的范围，使之能显示出所有的曲线的Y值
        qcustomplot.replot();  // 刷新
    });

    //波形线条初始化
    lineInit();
}

void WaveformDisplay::lineInit(){
    //新增波形
    QStringList colorStrList, nameList;
    colorStrList<<"green"<<"blue"<<"red";
    nameList<<"pitch:0°/s"<<"roll:0°/s"<<"yaw:0°/s";
    for (int var = 0; var < 3; ++var) {
        QPen pen;
        pen.setWidth(2);//设置线宽
        pen.setStyle(Qt::PenStyle::SolidLine);//设置为实线
        pen.setColor(QColor(colorStrList.at(var)));//设置线条颜色

        qcustomplot.addGraph();
        qcustomplot.graph(var)->setName(nameList.at(var));
        qcustomplot.graph(var)->setPen(pen);
        qcustomplot.graph(var)->setVisible(true);
    }

    //显示图例
    qcustomplot.legend->setMinimumSize(120,60);
    qcustomplot.legend->setVisible(true);
    //允许拖拽
    qcustomplot.setInteraction( QCP::iRangeDrag , true);
    //允许缩放
    qcustomplot.setInteraction( QCP::iRangeZoom , true);

}


void WaveformDisplay::paintGyroData(float gyroX, float gyroY, float gyroZ)
{
    if(!startFlag)
        return;

    //添加数据
    qcustomplot.graph(0)->addData(frameCount,gyroX);
    qcustomplot.graph(1)->addData(frameCount,gyroY);
    qcustomplot.graph(2)->addData(frameCount,gyroZ);
    qcustomplot.graph(0)->setName("pitch:" + QString::number(gyroX, 'f', 1) + "°/s");
    qcustomplot.graph(1)->setName("roll:" + QString::number(gyroY, 'f', 1) + "°/s");
    qcustomplot.graph(2)->setName("yaw:" + QString::number(gyroZ, 'f', 1) + "°/s");

    frameCount++;

    if(dynamicDisplayFlag){
        //刷新数据的时候才动态显示
        //动态x轴
        qcustomplot.xAxis->setRange(frameCount, 300, Qt::AlignHorizontal_Mask);
        //设置y轴范围
        QCPRange a = qcustomplot.yAxis->range();
        if(gyroX < a.lower)
            qcustomplot.yAxis->setRange(gyroX,a.upper);
        else if(gyroX > a.upper)
            qcustomplot.yAxis->setRange(a.lower,gyroX);

        if(gyroY < a.lower)
            qcustomplot.yAxis->setRange(gyroY,a.upper);
        else if(gyroY > a.upper)
            qcustomplot.yAxis->setRange(a.lower,gyroY);

        if(gyroZ < a.lower)
            qcustomplot.yAxis->setRange(gyroZ,a.upper);
        else if(gyroZ > a.upper)
            qcustomplot.yAxis->setRange(a.lower,gyroZ);
    }

    qcustomplot.replot(QCustomPlot::rpQueuedReplot); //刷新
}

#include "WaveformDisplay.h"
#include "extend/panelwidget.h"

/*--------------------------WaveformDisplay--------------------------------
 *  WaveformDisplay 波形显示实现
-----------------------------------------------------------------------*/
WaveformDisplay::WaveformDisplay(QWidget *parent)
    : QWidget{parent}
{
    //水平布局装一个tabwidget和一个widget
    QFrame* toplayoutFrame = new QFrame();
    QHBoxLayout* topLayout = new QHBoxLayout(toplayoutFrame);
    topLayout->setContentsMargins(2,3,5,3);
    topLayout->setSpacing(3);

    //定义颜色
    userColorStrList<<"paleturquoise"<<"darkcyan"<<"steelblue"<<"deepskyblue"
                     <<"darkslateblue"<<"brown"<<"forestgreen"
                     <<"gold"<<"darkorange"<<"orangered"<<"violet"
                     <<"darkviolet"<<"darkgoldenrod"<<"deeppink";

    //添加checkbox
    QList<QWidget *>* checkList = new QList<QWidget *>() ;
    for (int var = 0; var < 10; ++var) {
        QCheckBox* newCheckBox = new QCheckBox();
        newCheckBox->setObjectName("Var"+QString::number(var));
        newCheckBox->setText("Var"+QString::number(var)+" id=0x3" +QString::number(var)  );
        //组合样式表
        QString StyleSheetStr = "QCheckBox{background-color:" + userColorStrList.at(var) + "}";
        //设置样式表
        newCheckBox->setStyleSheet(StyleSheetStr);
        //新建的checkbox加入checkbox列表中方便查找使用
        checkList->append(newCheckBox);
        //链接信号和槽根据选择状态决定是否显示波形
        connect(newCheckBox, &QCheckBox::stateChanged , this , [=](){
            pQCustomPlot->graph(var)->setVisible(newCheckBox->isChecked());
            pQCustomPlot->replot(); //刷新
        });
    }

    //checkbox装不完添加滑动条
    //用户自定义数据checkbox
    PanelWidget* newWidget = new PanelWidget();
    //0x57 == 'W'
    newWidget->setObjectName("地址0x57");
    newWidget->setMargin(5);
    newWidget->setWidget(*checkList, 1);

    QTabWidget* checkWidget = new QTabWidget();
    checkWidget->addTab(newWidget, "地址0x57");

    //提升widget类为QCustomPlot类
    pQCustomPlot = new QCustomPlot();
    //设置波形显示背景
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor("#20B2AA"));
    plotGradient.setColorAt(1, QColor("#FFFACD"));
    pQCustomPlot->setBackground(plotGradient);

    topLayout->addWidget(checkWidget);
    topLayout->addWidget(pQCustomPlot);
    topLayout->setStretch(0,1);
    topLayout->setStretch(1,10);
    //底部水平布局装控制按钮
    QFrame* bottomLayoutFrame = new QFrame();
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomLayoutFrame);
    bottomLayout->setContentsMargins(2,2,2,2);
    bottomLayout->setSpacing(2);
    //配置按钮
    //开始显示按钮
    QPushButton* startWaveformButton = new QPushButton();
    startWaveformButton->setText("开始显示");
    connect(startWaveformButton,&QPushButton::clicked,pQCustomPlot,[=](){
        if(startWaveformButton->text() == "开始显示"){
            startWaveformButton->setText("暂停显示");
            startFlag = true;
        }else{
            startWaveformButton->setText("开始显示");
            startFlag = false;
        }
    });
    //清空波形按钮
    bottomLayout->addWidget(startWaveformButton);
    QPushButton* clearWaveformButton = new QPushButton();
    clearWaveformButton->setText("清空波形");
    connect(clearWaveformButton,&QPushButton::clicked,pQCustomPlot,[=](){
        int count=pQCustomPlot->graphCount();//获取曲线条数
        for(int i=0;i<count;++i)
        {
            pQCustomPlot->graph(i)->data().data()->clear();
        }
        pQCustomPlot->replot();
    });
    bottomLayout->addWidget(clearWaveformButton);
    //动态显示按钮
    QPushButton* dynamicDisplayButton = new QPushButton();
    dynamicDisplayButton->setText("停止跟随");
    connect(dynamicDisplayButton,&QPushButton::clicked,pQCustomPlot,[=](){
        if(dynamicDisplayButton->text() == "动态显示"){
            dynamicDisplayButton->setText("停止跟随");
            dynamicDisplayFlag = true;
        }else{
            dynamicDisplayButton->setText("动态显示");
            dynamicDisplayFlag = false;
        }
    });
    bottomLayout->addWidget(dynamicDisplayButton);
    //全显波形按钮
    QPushButton* seeAllLineButton = new QPushButton();
    seeAllLineButton->setText("波形全显");
    connect(seeAllLineButton,&QPushButton::clicked,pQCustomPlot,[=](){
        dynamicDisplayButton->setText("动态显示");
        dynamicDisplayFlag = false;
        pQCustomPlot->xAxis->rescale(true);//调整X轴的范围，使之能显示出所有的曲线的X值
        pQCustomPlot->yAxis->rescale(true);//调整Y轴的范围，使之能显示出所有的曲线的Y值
        pQCustomPlot->replot();  // 刷新
    });
    bottomLayout->addWidget(seeAllLineButton);

    //主布局设置
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2,0,2,0);
    mainLayout->setSpacing(2);
    mainLayout->addWidget(toplayoutFrame);
    mainLayout->addWidget(bottomLayoutFrame);

    waveformViewWidget = new QWidget();
    waveformViewWidget->setLayout(mainLayout);

    //波形线条初始化
    lineInit();
}

QWidget* WaveformDisplay::widget()
{
    return waveformViewWidget;
}

void WaveformDisplay::lineInit(){
    //新增10条固定地址波形
    for (int var = 0; var < 10; ++var) {
        QPen pen;
        pen.setWidth(2);//设置线宽
        pen.setStyle(Qt::PenStyle::SolidLine);//设置为实线
        pen.setColor(QColor(userColorStrList.at(var)));//设置线条颜色

        pQCustomPlot->addGraph();
        pQCustomPlot->graph(var)->setPen(pen);
        pQCustomPlot->graph(var)->setVisible(false);
    }

    //显示图例
    //    pQCustomPlot->legend->setVisible(true);
    //允许拖拽
    pQCustomPlot->setInteraction( QCP::iRangeDrag , true);
    //允许缩放
    pQCustomPlot->setInteraction( QCP::iRangeZoom , true);

}


void WaveformDisplay::paintUserData(double x ,double y , int num)
{
    //获取checkbox控件句柄
    static QList<QCheckBox*>  CheckBoxList = this->widget()->findChildren<QCheckBox*>();

    static bool isChange = false;
    if(startFlag){
        //添加数据
//        y = sin(x)*20*(num+1);
        pQCustomPlot->graph(num)->addData(x,y);
        isChange=true;
    }
    if(dynamicDisplayFlag){
        //刷新数据的时候才动态显示
        if(isChange){
            //动态x轴
            pQCustomPlot->xAxis->setRange(x-20, 300, Qt::AlignRight);
            //设置y轴范围
            QCPRange a = pQCustomPlot->yAxis->range();
            if(y < a.lower){
                pQCustomPlot->yAxis->setRange(y,a.upper);
            }
            if(y > a.upper){
                pQCustomPlot->yAxis->setRange(a.lower,y);
            }

        }
    }

    pQCustomPlot->replot(QCustomPlot::rpQueuedReplot); //刷新
    //    static QTimer* thisTimer = new QTimer();
    //    thisTimer->setTimerType(Qt::PreciseTimer);
    //    connect(thisTimer , &QTimer::timeout , this , [=](){
    //        if(isChange){
    //            pQCustomPlot->replot(QCustomPlot::rpQueuedReplot); //刷新
    //            isChange=false;
    //        }
    //    });
    //    thisTimer->start(1);

}

void WaveformDisplay::ProcessingMessages(uint8_t *pData, uint8_t len)
{
    uint8_t id = *(pData+2)-48;
    static int x=0;
    uint32_t data = 0 ;

    for (int var = 0; var < 4; var++) {
        data += *(pData+4+var)<<(8*(3-var));
    }
    qDebug()<<data;
    paintUserData(x, int(data) , id);
    x++;
}

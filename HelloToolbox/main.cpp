#include <QApplication>

#include <QToolButton>
#include <QWidget>
#include <QTextEdit>
#include <QSerialPort>
#include <QFile>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "serialport/SerialPortBase.h"
//#include "waveform/waveform.h"
#include "network/network.h"

QStackedWidget* pStackedWidget;

void StartWindowInit(QWidget *pWidget){
    QVBoxLayout* mainLayout = new QVBoxLayout(pWidget);
    //设置布局边距Policy
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    /*添加自定义界面组件 */
    pStackedWidget = new QStackedWidget();

    /*底部主界面栏*/
    // 提升layout控件为frame
    QFrame* lowLayoutFrame = new QFrame(pWidget);
    QHBoxLayout* lowLayout = new QHBoxLayout(lowLayoutFrame);

    //加入label显示基本信息，垂直布局
    QLabel* say = new QLabel();
    QString sayTex = {  "\nHello World\n"};
    say->setText(sayTex);
    lowLayout->addWidget(say);

    //配置按钮
    auto buttonInit = [=](QString name , QString iconPath){
        QToolButton* newTButton = new QToolButton(lowLayoutFrame);
        newTButton->setText(name);
        newTButton->setFocusPolicy(Qt::WheelFocus);
        QIcon icon;
        icon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
        newTButton->setIcon(icon);
        newTButton->setIconSize(QSize(40, 40));
        newTButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        newTButton->setAutoRaise(true);
        lowLayout->addWidget(newTButton);
        return newTButton;
    };

    //创建按钮并绑定槽函数
    static QToolButton* serialport = buttonInit("串口配置",":/icon/ser.ico");
    QObject::connect(serialport ,&QPushButton::clicked,pWidget,[pWidget]() {
        pStackedWidget->setCurrentIndex(0);
    });
    static QToolButton* waveform = buttonInit("波形显示",":/icon/bx.ico");
    QObject::connect(waveform ,&QPushButton::clicked,pWidget,[pWidget]() {
        pStackedWidget->setCurrentIndex(1);
    });
    static QToolButton* network = buttonInit("网络通信",":/icon/message.ico");
    QObject::connect(waveform ,&QPushButton::clicked,pWidget,[pWidget]() {
        pStackedWidget->setCurrentIndex(2);
    });
    static QToolButton* hidden = buttonInit("隐藏窗口",":/icon/small.ico");
    QObject::connect(hidden ,&QPushButton::clicked,pWidget,[pWidget]() {
        pWidget->setWindowState(Qt::WindowMinimized);
    });
    static QToolButton* close = buttonInit("退出软件",":/icon/close.ico");
    QObject::connect(close ,&QPushButton::clicked,pWidget,[&]() {
        free(serialport);
        free(waveform);
        free(network);
        free(hidden);
        free(close);
        qDebug()<<pWidget->close();
        qDebug()<<"free memory";
    });

    //加入基础上面两个基础控件到主界面布局
    mainLayout->addWidget(pStackedWidget);
    mainLayout->addWidget(lowLayoutFrame);
    //设置布局排版
    mainLayout->setStretch(0,9);
    mainLayout->setStretch(1,1);
    pWidget->setLayout(mainLayout);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.resize(800, 450);
    QMetaObject::connectSlotsByName(&w);

    //主窗口初始化
    StartWindowInit(&w);
    // 设置应用程序图标
    w.setWindowIcon(QIcon(":/icon/software.ico"));

    //添加自定义界面
    SerialPortBase *serialport = new SerialPortBase() ;
    pStackedWidget->addWidget(serialport->Widget());

//    WaveformDisplay *waveform = new WaveformDisplay() ;
//    pStackedWidget->addWidget(waveform->Widget());

//    QObject::connect(serialport, &SerialPortBase::gyroDataupdate, waveform, &WaveformDisplay::paintGyroData);

    NetworkManage *network = new NetworkManage() ;
    QThread *NetworkThread = new QThread;
    network->moveToThread(NetworkThread);
    NetworkThread->start();

    pStackedWidget->addWidget(network->Widget());

    pStackedWidget->setCurrentIndex(0);

    w.show();
    return a.exec();
}

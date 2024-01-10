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
#include "waveform/waveform.h"
#include "network/network.h"

void StartWindowInit(QStackedWidget *pStackedWidget, QWidget *pWidget){
    static QVBoxLayout mainLayout;
    //设置布局边距Policy
    mainLayout.setContentsMargins(0,0,0,0);
    mainLayout.setSpacing(0);

    /*底部主界面栏*/
    static QFrame lowLayoutFrame(pWidget);
    static QHBoxLayout lowLayout(&lowLayoutFrame);

    //加入label显示基本信息，垂直布局
    QLabel* say = new QLabel();
    QString sayTex = {  "\nHello World\n"};
    say->setText(sayTex);
    lowLayout.addWidget(say);

    //配置按钮
    auto ButtonInit = [=](QToolButton* button, QString name , QString iconPath){
        QToolButton* newTButton = new QToolButton();
        button->setText(name);
        button->setFocusPolicy(Qt::WheelFocus);
        QIcon icon;
        icon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
        button->setIcon(icon);
        button->setIconSize(QSize(40, 40));
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        button->setAutoRaise(true);
        lowLayout.addWidget(button);
    };

    //创建按钮并绑定槽函数
    static QToolButton serialport;
    ButtonInit(&serialport, "串口配置",":/icon/ser.ico");
    QObject::connect(&serialport ,&QPushButton::clicked,pWidget,[&]() {
        pStackedWidget->setCurrentIndex(0);
    });
    static QToolButton waveform;
    ButtonInit(&waveform, "波形显示",":/icon/bx.ico");
    QObject::connect(&waveform ,&QPushButton::clicked,pWidget,[&]() {
        pStackedWidget->setCurrentIndex(1);
    });
    static QToolButton network;
    ButtonInit(&network, "网络通信",":/icon/message.ico");
    QObject::connect(&network ,&QPushButton::clicked,pWidget,[&]() {
        pStackedWidget->setCurrentIndex(2);
    });
    static QToolButton hidden;
    ButtonInit(&hidden, "隐藏窗口",":/icon/small.ico");
    QObject::connect(&hidden ,&QPushButton::clicked,pWidget,[&]() {
        pWidget->setWindowState(Qt::WindowMinimized);
    });
    static QToolButton close;
    ButtonInit(&close, "退出软件",":/icon/close.ico");
    QObject::connect(&close ,&QPushButton::clicked,pWidget,[&]() {
        qDebug()<<pWidget->close();
    });

    //加入基础上面两个基础控件到主界面布局
    mainLayout.addWidget(pStackedWidget);
    mainLayout.addWidget(&lowLayoutFrame);
    //设置布局排版
    mainLayout.setStretch(0,9);
    mainLayout.setStretch(1,1);
    pWidget->setLayout(&mainLayout);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStackedWidget stackedwidget;

    //添加自定义界面
    SerialPortBase serialport;
    stackedwidget.addWidget(serialport.Widget());

    WaveformDisplay waveform;
    stackedwidget.addWidget(waveform.Widget());

    QObject::connect(&serialport, &SerialPortBase::gyroDataupdate, &waveform, &WaveformDisplay::paintGyroData);

    NetworkManage network;
    QThread *NetworkThread = new QThread;
    network.moveToThread(NetworkThread);
    NetworkThread->start();
    stackedwidget.addWidget(network.Widget());

    //主界面初始化
    QWidget mainwidget;
    QMetaObject::connectSlotsByName(&mainwidget);
    mainwidget.resize(800, 450);
    StartWindowInit(&stackedwidget, &mainwidget);

    mainwidget.setFont(QFont("楷体", 13));
    mainwidget.setWindowIcon(QIcon(":/icon/software.ico"));
    mainwidget.show();

    return a.exec();
}

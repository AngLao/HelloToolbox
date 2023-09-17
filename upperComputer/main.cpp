#include "main.h"
#include <QApplication>
#include <QTextEdit>
#include <QToolButton>

#include "SerialPort/SerialPortBase.h"
#include "Waveform/WaveformDisplay.h"

//主函数入口
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(800, 450);
    QMetaObject::connectSlotsByName(this);

    //主窗口初始化
    startWindowInit();

    //添加自定义界面
    class SerialPortBase *pSerialPortBase = new class SerialPortBase() ;
    class WaveformDisplay *pWaveformDisplay = new class WaveformDisplay() ;

    pStackedWidget->addWidget(pSerialPortBase->Widget());
    pStackedWidget->addWidget(pWaveformDisplay->Widget());

    pStackedWidget->setCurrentIndex(0);

}


void Widget::startWindowInit(void){
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    //设置布局边距Policy
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    /*添加自定义界面组件 */
    pStackedWidget = new QStackedWidget();
    /*底部主界面栏*/
    // 提升layout控件为frame
    QFrame* lowLayoutFrame = new QFrame(this);
    QHBoxLayout* lowLayout = new QHBoxLayout(lowLayoutFrame);

    //加入label显示基本信息，垂直布局
    QLabel* say = new QLabel();
    QString sayTex = {  "version: 1.0 \n"
                        "欢迎使用Hello ToolBox \n"};
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
    auto addTButton = [=]{
        static QToolButton* serialConf = buttonInit("串口配置",":/Icon/ser.ico");
        connect(serialConf ,&QPushButton::clicked,this,[this]() {
            pStackedWidget->setCurrentIndex(0);
        });
        static QToolButton* dataView = buttonInit("数据面板",":/Icon/data.ico");
        connect(dataView ,&QPushButton::clicked,this,[this]() {
            pStackedWidget->setCurrentIndex(1);
        });
        static QToolButton* protocolResolution = buttonInit("协议解析",":/Icon/tell.ico");
        connect(protocolResolution ,&QPushButton::clicked,this,[this]() {
            pStackedWidget->setCurrentIndex(2);
        });
        static QToolButton* userConf = buttonInit("用户配置",":/Icon/user.ico");
        connect(userConf ,&QPushButton::clicked,this,[this]() {
            pStackedWidget->setCurrentIndex(3);

        });
        static QToolButton* waveformDisplay = buttonInit("波形显示",":/Icon/bx.ico");
        connect(waveformDisplay ,&QPushButton::clicked,this,[this]() {
            pStackedWidget->setCurrentIndex(4);
        });
        static QToolButton* sendCmd = buttonInit("网络通讯",":/Icon/message.ico");
        connect(sendCmd ,&QPushButton::clicked,this,[this]() {
        });
        static QToolButton* debugManual = buttonInit("参数配置",":/Icon/txxy.ico");
        connect(debugManual ,&QPushButton::clicked,this,[=]() {
            pStackedWidget->setCurrentIndex(5);
        });
        static QToolButton* beSmall = buttonInit("隐藏窗口",":/Icon/small.ico");
        connect(beSmall ,&QPushButton::clicked,this,[=]() {
            this->setWindowState(Qt::WindowMinimized);
        });
        static QToolButton* closeSoftware = buttonInit("退出软件",":/Icon/close.ico");
        connect(closeSoftware ,&QPushButton::clicked,this,[=]() {
            this->close();
            //     delete ui;
        });

    };
    addTButton();
    //加入基础上面两个基础控件到主界面布局
    mainLayout->addWidget(pStackedWidget);
    mainLayout->addWidget(lowLayoutFrame);
    //设置布局排版
    mainLayout->setStretch(0,9);
    mainLayout->setStretch(1,1);
    this->setLayout(mainLayout);

}

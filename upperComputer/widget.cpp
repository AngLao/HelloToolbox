#include "widget.h"
#include <QApplication>
#include <QTextEdit>
#include <QToolButton>

#include "uiinit.h"
#include "SerialPortBase.h"
#include "WaveformDisplay.h"
#include "ParameterConfiguration.h"

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
    this->resize(940, 450);
    QMetaObject::connectSlotsByName(this);
    //设置无边框
//    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint);

    //主窗口初始化
    startWindowInit();
    //添加自定义界面
    SerialPortBase* SerialPortBase = new class SerialPortBase(this);
    dataView* dataView = new class dataView();
    frameView* frameView = new class frameView();
    userConfView* userConfView = new  class userConfView();
    WaveformDisplay* WaveformDisplay = new class WaveformDisplay();
    ParameterConfiguration* ParameterConfiguration = new class ParameterConfiguration();
    m_pStackedWidget->addWidget(SerialPortBase);
    m_pStackedWidget->addWidget(dataView->widget());
    m_pStackedWidget->addWidget(frameView->widget());
    m_pStackedWidget->addWidget(userConfView->widget());
    m_pStackedWidget->addWidget(WaveformDisplay->widget());
    m_pStackedWidget->addWidget(ParameterConfiguration->widget());

    m_pStackedWidget->setCurrentIndex(0);
//    //原始帧数据通过信号发送给userConfView类按相应配置进行解析
//    connect(pqAgreement, &qAgreement::frameQuoteSignal,userConfView,&userConfView::combinationUserData);
//    //解析完成的最终结果发送到进行显示
//    connect(userConfView, &userConfView::showframe,frameView,&frameView::addItem);
//    connect(userConfView, &userConfView::refreshDataView,dataView,&dataView::refreshUserView);

    //配置数据发送链接
    connect(ParameterConfiguration, &ParameterConfiguration::sendPackData,SerialPortBase,&SerialPortBase::SendData);
    //显示波形
    connect(SerialPortBase, &SerialPortBase::drawWaveform, WaveformDisplay, &WaveformDisplay::ProcessingMessages);


//    qssInit(":/qss/test.css");
}


void Widget::startWindowInit(void){
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    //设置布局边距Policy
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    /*添加自定义界面组件 */
    m_pStackedWidget = new QStackedWidget();
    /*底部主界面栏*/
    // 提升layout控件为frame
    QFrame* lowLayoutFrame = new QFrame(this);
    QHBoxLayout* lowLayout = new QHBoxLayout(lowLayoutFrame);
    //设置布局边距
    lowLayout->setContentsMargins(3,3,3,3);

    //加入label显示基本信息，垂直布局
    QFrame* sayLayoutFrame = new QFrame();
    QVBoxLayout* sayLayout = new QVBoxLayout(sayLayoutFrame);
    sayLayout->setContentsMargins(3,3,3,3);
    QLabel* say = new QLabel();
    say->setText("收发计数:");
    sayLayout->addWidget(say);
    //串口接收计数label
    QHBoxLayout* sayInLayout = new QHBoxLayout();
    QLabel* serialIn = new QLabel();
    serialIn->setText("串口接收计数:");
    QLabel* serialInValue = new QLabel();
    serialInValue->setObjectName("串口接收计数值");
    serialInValue->setText(QString::number(0));
    sayInLayout->addWidget(serialIn);
    sayInLayout->addWidget(serialInValue);
    sayInLayout->setStretch(0,1);
    sayInLayout->setStretch(1,99);
    //串口发送计数label
    QHBoxLayout* sayOutLayout = new QHBoxLayout();
    QLabel* serialOut = new QLabel();
    serialOut->setText("串口发送计数:");
    QLabel* serialOutValue = new QLabel();
    serialOutValue->setObjectName("串口发送计数值");
    serialOutValue->setText(QString::number(0));
    sayOutLayout->addWidget(serialOut);
    sayOutLayout->addWidget(serialOutValue);
    sayOutLayout->setStretch(0,1);
    sayOutLayout->setStretch(1,99);

    sayLayout->addLayout(sayInLayout);
    sayLayout->addLayout(sayOutLayout);

    lowLayout->addWidget(sayLayoutFrame);

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
            m_pStackedWidget->setCurrentIndex(0);
        });
        static QToolButton* dataView = buttonInit("数据面板",":/Icon/data.ico");
        connect(dataView ,&QPushButton::clicked,this,[this]() {
            m_pStackedWidget->setCurrentIndex(1);

        });
        static QToolButton* protocolResolution = buttonInit("协议解析",":/Icon/tell.ico");
        connect(protocolResolution ,&QPushButton::clicked,this,[this]() {
            m_pStackedWidget->setCurrentIndex(2);
        });
        static QToolButton* userConf = buttonInit("用户配置",":/Icon/user.ico");
        connect(userConf ,&QPushButton::clicked,this,[this]() {
            m_pStackedWidget->setCurrentIndex(3);

        });
        static QToolButton* waveformDisplay = buttonInit("波形显示",":/Icon/bx.ico");
        connect(waveformDisplay ,&QPushButton::clicked,this,[this]() {
            m_pStackedWidget->setCurrentIndex(4);
        });
        static QToolButton* sendCmd = buttonInit("网络通讯",":/Icon/message.ico");
        connect(sendCmd ,&QPushButton::clicked,this,[this]() {
        });
        static QToolButton* debugManual = buttonInit("参数配置",":/Icon/txxy.ico");
        connect(debugManual ,&QPushButton::clicked,this,[=]() {
            m_pStackedWidget->setCurrentIndex(5);
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
    mainLayout->addWidget(m_pStackedWidget);
    mainLayout->addWidget(lowLayoutFrame);
    //设置布局排版
    mainLayout->setStretch(0,9);
    mainLayout->setStretch(1,1);
    this->setLayout(mainLayout);

}

void Widget::qssInit(QString filePath){
    //加载样式表
    if (!filePath.isEmpty()) {
        QFile file(filePath);

        if (file.open(QFile::ReadOnly)) {
            QString qss = file.readAll();
            //            QString qss = QLatin1String(file.readAll());
            this->setStyleSheet(qss);
        }
    }
}

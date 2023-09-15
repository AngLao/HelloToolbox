#ifndef UIINIT_H
#define UIINIT_H

#include "analyse.h"
#include "qcustomplot.h"
#include <QObject>
#include <QLabel>
#include <QLine>
#include <QTabWidget>
#include <QVBoxLayout>

#include <QTableWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QHeaderView>


//------------------------uiInit-------------------
class uiInit : public QObject
{
    Q_OBJECT
public:
    uiInit(){};

};

//------------------------dataView-------------------
class dataView : public QObject
{
    Q_OBJECT
public:
    explicit dataView();
    ~dataView(){};
    QWidget *widget();
    void setViewData(QString frmDeviceWidgetName, QStringList *mQStringlis);
    void setLabelValue(QString labelName, double value);
    void refreshUserView(int i , double value);
private:
    void QStringListInit();
    QTabWidget* dataViewTabWidget;
    QVector<QStringList*> flyList;
    QVector<QStringList*> userList;
};

//------------------------userConfView-------------------
//-------------------------------------------------------
//自定义帧结构体
enum user_data_type{
    Uint8=1, Uint16, Int16, Int32, Uint32
};

enum transmission_scaling{
    //缩放10的几次方倍数
    M1=0, M2, M3, M4, M5 , M6, M7, M8, M9, M10
};

typedef struct bitConf1         //一个数据位
{
    user_data_type udt;         //一个数据位的数据类型
    transmission_scaling ts;    //一个数据位的传输缩放
}bituserConf;

typedef struct  Conf1
{
    unsigned int check_sta;      //此帧选择状态
    bitConf1 bit[10];            //此帧各个位的情况
}frameuserConf;                  //一帧自定义配置
//---------------------------------------------------------
//数据容器结构体
enum whatFrame{
    F1=0, F2 , F3 ,F4 ,F5 ,F6 ,F7,F8 ,F9 ,FA
};
enum whatBit{
    B1=0, B2 , B3 ,B4 ,B5 ,B6 ,B7,B8 ,B9 ,B10
};
typedef struct Container        //一个数据容器
{
    whatFrame frameNum;         //数据容器对应的帧
    whatBit bitNum;             //数据容器对应的帧数据位置
}dataContainer;
//----------------------------------------------------------
//用户自定义帧最终解析结果
union databit
{
    unsigned char Uint8 ;
    unsigned short Uint16 ;
    short Int16 ;
    int   Int32;
    unsigned int   Uint32;
};
typedef struct frameResult                  //最终解析出来的帧数据结构
{
    //帧内容
    unsigned char  head = 0x00;             //帧头
    unsigned char  addr = 0x00;             //帧地址
    unsigned char  id   = 0x00;             //帧ID
    unsigned char  len  = 0x00;             //帧数据长度
    databit data[10];
    unsigned char  sc   = 0x00;             //帧和校验
    unsigned char  ac   = 0x00;             //帧附加校验
}finalFrame;
//------------------------------------------------------------------------------------

class userConfView : public QWidget
{
    Q_OBJECT

public:
    explicit userConfView(QWidget *parent = nullptr);
    ~userConfView(){};

    QWidget *widget();

    frameuserConf  userConf[10];//F1-FA 共十帧配置
    dataContainer  dataConf[20];//1-10  共十个数据容器
    double         uesrDataBuf[20] ;//储存的用户数据值

    finalFrame uesrFinalFrame;

    void readUserConf();
    void readDataConf();
    void combinationUserData(void *mframe);
    void refreshView();
signals:

    //textedit刷新信号
    void showframe(finalFrame *pResult, int i);
    //数据看板刷新
    void refreshDataView(int i , double value);
    //绘图数据信号
    void refreshPrint(double key ,double value , int num);
private:
    QWidget* userConfViewWidger;

    void setUserConfView(QTreeWidget *frameconf);
    void setDataConfView(QTableWidget* dataconf);

};


//------------------------frameView-------------------
class frameView : public QObject
{
    Q_OBJECT
public:

    explicit frameView();
    ~frameView(){};

    QWidget *widget();
    void addItem(finalFrame *pResult, int i);
private:
    QWidget* frameViewWidget;
};

#endif // UIINIT_H

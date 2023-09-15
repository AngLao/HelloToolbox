#include "uiinit.h"
#include "extend/panelwidget.h"
#include "qcombobox.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QTextEdit>


//#include "opencv2/opencv.hpp"
/*---------------------------dataView-----------------------------------
 *  dataView数据面板类ui实现
-----------------------------------------------------------------------*/
dataView::dataView()
{
    /*-----------------------------------------------------------------------
     *  根据QStringList需求创建一个用户数据标签
     *  每个标签由多个垂直分布的label组成
     *  第一个标签下和最后一个标签上有分割线条
     *  @parame mQStringlis 根据列表情况生成标签
    -----------------------------------------------------------------------*/
    auto newFrmDevice = [](QStringList* mQStringlis){
        QWidget* frmDeviceWidget = new QWidget();
        //设置QWidget名称，要改变标签板内数据是通过查找名称改变
        frmDeviceWidget->setObjectName(mQStringlis->at(0));
        //数据显示框为垂直布局,提升该布局为frame类，使其带边框
        QFrame* layoutFrame = new QFrame();
        QVBoxLayout* thisLayout = new QVBoxLayout(layoutFrame);

        //根据需求创建数据标签
        for(int i=0; i < mQStringlis->size() ; i++)
        {
            QLabel *Label = new QLabel(layoutFrame);
            //设置名称，方便查找 可单独修改标签值
            if(i != mQStringlis->size()-1){
                Label->setObjectName(mQStringlis->at(i));
            }else{//最后标签名称XX频率
                Label->setObjectName(mQStringlis->at(0)+mQStringlis->at(i));
                //qDebug()<<Label->objectName();
            }
            Label->setText(mQStringlis->at(i));
            thisLayout->addWidget(Label);
            //加分割线
            if(i==0 || i == mQStringlis->size()-2 ){
                QFrame* line = new QFrame();
                line->setMaximumSize(QSize(16777215, 1));
                line->setFrameShape(QFrame::HLine);
                thisLayout->addWidget(line);
            }
        }
        //创建一个布局显示提升为frame的布局类
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0,0,0,0);
        mainLayout->setSpacing(0);
        mainLayout->addWidget(layoutFrame);
        frmDeviceWidget->setLayout(mainLayout);

        return frmDeviceWidget;
    };
    /*-----------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------
     * 根据widget List生成带滑动条的widget阵列
    -----------------------------------------------------------------------*/
    auto newPanelWidget = [](QList<QWidget *>* frms){
        PanelWidget* newWidget = new PanelWidget();
        newWidget->setMargin(1);
        newWidget->setSpacing(12);
        newWidget->setWidget(*frms, 4);
        return newWidget;
    };
    /*-----------------------------------------------------------------------*/

    QStringListInit();
    //内置数据类型面板
    QList<QWidget *>* flyFrms = new QList<QWidget *>();
    for (int i = 0; i < flyList.size(); i++) {
        flyFrms->append(newFrmDevice(flyList.at(i)) );
    }

    //自定义数据类型面板
    QList<QWidget *>* userFrms = new QList<QWidget *>();    //创建飞行数据面板
    for (int i = 0; i < userList.size(); i++) {
        userFrms->append(newFrmDevice(userList.at(i)) );
    }
    //dataViwe不同功能块数据面板用Tab widget装载
    dataViewTabWidget = new QTabWidget();
    dataViewTabWidget->setTabShape(QTabWidget::Triangular  );
    dataViewTabWidget->addTab(newPanelWidget(flyFrms),"内置数据");
    dataViewTabWidget->addTab(newPanelWidget(userFrms),"用户数据");

}

//返回此ui widget句柄
QWidget *dataView::widget()
{
    return dataViewTabWidget;
}

//设置一个标签的值
void dataView::setLabelValue(QString labelName , double value){
    QLabel* thisLabel = dataViewTabWidget->findChild<QLabel*>(labelName);
    if(thisLabel == nullptr){
        qDebug()<< "cant find thisLabel"<<Qt::endl;
    }else{
        thisLabel->setText(thisLabel->objectName()+" = "+QString::number(value));
    }
}

//刷新用户数据面板槽函数
void dataView::refreshUserView(int i, double value)
{
    auto getUserLabelList = [=](){
        QList<QLabel*> userLabelList;
        for (int var = 0; var < 20; ++var) {
            QLabel* thisLabel = dataViewTabWidget->findChild<QLabel*>("value"+QString::number(var+1));
            qDebug()<<thisLabel;
            userLabelList.append(thisLabel);
        }
        return userLabelList;
    };

    static QList<QLabel*> userLabelList = getUserLabelList() ;

    //先找到面板位置
    userLabelList[i]->setText(userLabelList[i]->objectName()+" = "+QString::number(value));
    //    setLabelValue("value"+QString::number(i+1) ,value );
}

//设置一个面板中的所有数据
void dataView::setViewData(QString frmDeviceWidgetName ,QStringList* mQStringlis)
{
    //按名称寻找对应数据板块
    QWidget* findResult = dataViewTabWidget->findChild<QWidget*>(frmDeviceWidgetName);
    if(findResult != nullptr){
        //获取板块中label列表
        QList<QLabel*> thisLabelList  =  findResult->findChildren<QLabel*>();
        //         qDebug()<<thisLabelList.size()<<Qt::endl;
        for (int var = 0; var < thisLabelList.size(); ++var) {
            //            qDebug()<<thisLabelList.at(var)->text()<<Qt::endl;
            //修改显示值
            thisLabelList.at(var)->setText(mQStringlis->at(var));
        }

    }else{
        qDebug()<< "cant find"<<Qt::endl;
    }
}

//初始化各个面板显示数据名称
void dataView::QStringListInit(){
    //-----------------FLYDATA------------------
    static QStringList Id0x01 ;
    Id0x01<<"Id : 0x01"\
         <<"accX"<<"accY"<<"accZ"<<"gyrX"<<"gyrY"<<"gyrZ"<<"sta_shock" \
        <<"频率";
    flyList.append( &Id0x01) ;

    static QStringList Id0x02 ;
    Id0x02<<"Id : 0x02"\
         <<"magX"<<"magY"<<"magZ"<<"AltBar"<<"tmp"<<"staBar"<<"staMag"\
        <<"频率";
    flyList.append( &Id0x02) ;

    static QStringList Id0x03 ;
    Id0x03<<"Id : 0x03"\
         <<"roll"<<"pit"<<"yaw"<<"sta_fusion"\
        <<"频率";
    flyList.append( &Id0x03) ;

    static QStringList Id0x04 ;
    Id0x04<<"Id : 0x04"\
         <<"attV0"<<"attV1"<<"attV2"<<"attV3"\
        <<"频率";
    flyList.append( &Id0x04) ;

    //-----------------USERDATA------------------

    static QStringList USERDATA1 ;
    USERDATA1<<"USERDATA1"<<"value1"<<"频率";
    userList.append( &USERDATA1) ;

    static QStringList USERDATA2 ;
    USERDATA2<<"USERDATA2"<<"value2"<<"频率";
    userList.append( &USERDATA2) ;

    static QStringList USERDATA3 ;
    USERDATA3<<"USERDATA3"<<"value3"<<"频率";
    userList.append( &USERDATA3) ;

    static QStringList USERDATA4 ;
    USERDATA4<<"USERDATA4"<<"value4"<<"频率";
    userList.append( &USERDATA4) ;

    static QStringList USERDATA5 ;
    USERDATA5<<"USERDATA5"<<"value5"<<"频率";
    userList.append( &USERDATA5) ;

    static QStringList USERDATA6 ;
    USERDATA6<<"USERDATA6"<<"value6"<<"频率";
    userList.append( &USERDATA6) ;

    static QStringList USERDATA7 ;
    USERDATA7<<"USERDATA7"<<"value7"<<"频率";
    userList.append( &USERDATA7) ;

    static QStringList USERDATA8 ;
    USERDATA8<<"USERDATA8"<<"value8"<<"频率";
    userList.append( &USERDATA8) ;

    static QStringList USERDATA9 ;
    USERDATA9<<"USERDATA9"<<"value9"<<"频率";
    userList.append( &USERDATA9) ;

    static QStringList USERDATA10 ;
    USERDATA10<<"USERDATA10"<<"value10"<<"频率";
    userList.append( &USERDATA10) ;

    static QStringList USERDATA11 ;
    USERDATA11<<"USERDATA11"<<"value11"<<"频率";
    userList.append( &USERDATA11) ;

    static QStringList USERDATA12 ;
    USERDATA12<<"USERDATA12"<<"value12"<<"频率";
    userList.append( &USERDATA12) ;

    static QStringList USERDATA13 ;
    USERDATA13<<"USERDATA13"<<"value13"<<"频率";
    userList.append( &USERDATA13) ;

    static QStringList USERDATA14 ;
    USERDATA14<<"USERDATA14"<<"value14"<<"频率";
    userList.append( &USERDATA14) ;

    static QStringList USERDATA15 ;
    USERDATA15<<"USERDATA15"<<"value15"<<"频率";
    userList.append( &USERDATA15) ;

    static QStringList USERDATA16 ;
    USERDATA16<<"USERDATA16"<<"value16"<<"频率";
    userList.append( &USERDATA16) ;

    static QStringList USERDATA17 ;
    USERDATA17<<"USERDATA17"<<"value17"<<"频率";
    userList.append( &USERDATA17) ;

    static QStringList USERDATA18 ;
    USERDATA18<<"USERDATA18"<<"value18"<<"频率";
    userList.append( &USERDATA18) ;

    static QStringList USERDATA19 ;
    USERDATA19<<"USERDATA19"<<"value19"<<"频率";
    userList.append( &USERDATA19) ;

    static QStringList USERDATA20 ;
    USERDATA20<<"USERDATA20"<<"value20"<<"频率";
    userList.append( &USERDATA20) ;
}


/*--------------------------frameView------------------------------------
 *  frameView接收到的帧数据面板类ui实现
-----------------------------------------------------------------------*/
frameView::frameView()
{

    frameViewWidget = new QWidget();
    QTableWidget* frameView = new QTableWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(2);
    mainLayout->addWidget(frameView);
    frameViewWidget->setLayout(mainLayout);

    QStringList tableHeaderList;
    tableHeaderList<<"HEAD"<<"ADRE"<<"  ID  "<<" LEN "
                  <<"DATA1"<<"DATA2"<<"DATA3"<<"DATA4"<<"DATA5"
                 <<"DATA6"<<"DATA7"<<"DATA8"<<"DATA9"<<"DATA10"
                <<"  SC  "<<"  AC  ";
    //表头列文本设置
    frameView->setColumnCount(tableHeaderList.size());
    for (int i=0;i<tableHeaderList.size();i++) {
        frameView->setHorizontalHeaderItem(i,new QTableWidgetItem(tableHeaderList.at(i)));
        frameView->setColumnWidth(i,frameView->width());
    }
    //    frameView->horizontalHeader()->setStretchLastSection(true);
    frameView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //    frameView->verticalHeader()->setMinimumSectionSize(15);

    frameView->setSelectionBehavior(QAbstractItemView::SelectRows);//行选中
    frameView->setShowGrid(false);//不显示网格
    //    frameView->setFocusPolicy(Qt::NoFocus); //去除选中虚线框
    frameView->verticalHeader()->setVisible(false); //设置垂直头不可见
    frameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    frameView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能编写
    //    frameView->setSelectionMode(QAbstractItemView::SingleSelection);//只能单次选中


}

QWidget *frameView::widget()
{
    return frameViewWidget;
}

void frameView::addItem(finalFrame *pResult , int len)
{
    //获取数据容器配置窗口句柄
    static QTableWidget* frameView = this->widget()->findChild<QTableWidget*>();
    static int RowCont = 0;
    frameView->insertRow(RowCont);//增加一行

    //    static QVector<QTableWidgetItem*> itemVector;
    //    itemVector.append(new QTableWidgetItem( QString::number(pResult->head , 16).toUpper()) );
    //    itemVector.append(new QTableWidgetItem( QString::number(pResult->addr , 16).toUpper()) );
    //    itemVector.append(new QTableWidgetItem( QString::number(pResult->id , 16).toUpper()) );
    //    itemVector.append(new QTableWidgetItem( QString::number(len , 16).toUpper()) );
    //    itemVector.append(new QTableWidgetItem( QString::number(pResult->sc , 16).toUpper()) );
    //    itemVector.append(new QTableWidgetItem( QString::number(pResult->ac , 16).toUpper()) );
    //    //插入元素
    //    for (int var = 0; var < pResult->len; ++var) {
    //        itemVector.append(new QTableWidgetItem( QString::number(pResult->data[var].Uint32 , 16).toUpper() ));
    //    }
    //插入元素
    //    frameView->setItem(RowCont,0,new QTableWidgetItem( QString::number(pResult->head , 16).toUpper() ));
    //    frameView->setItem(RowCont,1,new QTableWidgetItem( QString::number(pResult->addr , 16).toUpper() ));
    //    frameView->setItem(RowCont,2,new QTableWidgetItem( QString::number(pResult->id , 16).toUpper() ));
    //    frameView->setItem(RowCont,3,new QTableWidgetItem( QString::number(i , 16).toUpper() ));
    //    for (int var = 0; var < pResult->len; ++var) {
    //        frameView->setItem(RowCont,4+var,new QTableWidgetItem( QString::number(pResult->data[var].Uint32 , 16).toUpper() ));
    //    }
    //    frameView->setItem(RowCont,14,new QTableWidgetItem( QString::number(pResult->sc , 16).toUpper() ));
    //    frameView->setItem(RowCont,15,new QTableWidgetItem( QString::number(pResult->ac , 16).toUpper() ));
    RowCont++;
    frameView->scrollToBottom();
}


/*--------------------------userConfView--------------------------------
 *  userConfView 自定义协议解析类ui实现
-----------------------------------------------------------------------*/

userConfView::userConfView(QWidget *parent)
{
    userConfViewWidger = new QWidget(parent);
    //垂直布局装两GroupBox
    QFrame* layoutFrame = new QFrame();
    QVBoxLayout* groupLayout = new QVBoxLayout(layoutFrame);

    QTreeWidget* frameconfTreeWidget = new QTreeWidget();
    setUserConfView(frameconfTreeWidget);

    QGroupBox* frameconfBox = new QGroupBox(layoutFrame);
    frameconfBox->setTitle("自定义用户帧配置");
    QVBoxLayout* frameconfBoxLayout = new QVBoxLayout();
    frameconfBoxLayout->addWidget(frameconfTreeWidget);
    frameconfBox->setLayout(frameconfBoxLayout);

    QTableWidget* dataconfTreeWidget = new QTableWidget(frameconfBox);
    setDataConfView(dataconfTreeWidget);

    QGroupBox* dataconfBox = new QGroupBox(layoutFrame);
    dataconfBox->setTitle("数据容器配置");
    QVBoxLayout* dataconfBoxLayout = new QVBoxLayout();
    dataconfBoxLayout->addWidget(dataconfTreeWidget);
    dataconfBox->setLayout(dataconfBoxLayout);

    groupLayout->addWidget(frameconfBox);
    groupLayout->addWidget(dataconfBox);
    groupLayout->setContentsMargins(0,0,0,0);
    groupLayout->setSpacing(0);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(2);
    mainLayout->addWidget(layoutFrame);
    userConfViewWidger->setLayout(mainLayout);

    if(parent!=nullptr){
        userConfViewWidger->resize(parent->size());
    }
}
//返回窗口句柄
QWidget *userConfView::widget()
{
    return userConfViewWidger;
}

//配置自定义数据帧界面
void userConfView::setUserConfView(QTreeWidget*  frameconf)
{
    //绘制自定义用户帧表
    QList<QTreeWidgetItem *> items;
    frameconf->setColumnCount(4);
    frameconf->setHeaderLabels(QStringList()<<"用户帧"<<"数据位"<<"数据类型"<<"传输缩放");

    for(int i =1   ; i<=10 ; i++)
    {
        if(i<10){
            //父节点设置
            QTreeWidgetItem * item=new QTreeWidgetItem(QStringList()<<"F"+QString::number(i));
            frameconf->addTopLevelItem(item);
            //使能选择框设置
            item->setCheckState(1, Qt::Unchecked);
            item->setText(1,  "使能该帧");
            //子节点设置
            for(int x =0 ; x<10 ; x++)
            {
                QTreeWidgetItem * childitem=new QTreeWidgetItem(QStringList()<<"F"+QString::number(i));
                item->addChild(childitem);

                childitem->setText(1,QString::number(x+1));

                //设置数据类型下拉框控件
                QComboBox *frameBox = new QComboBox;
                frameBox->addItem("Uint8" );
                frameBox->addItem("Uint16" );
                frameBox->addItem("Int16" );
                frameBox->addItem("Int32" );
                frameBox->addItem("Uint32" );
                frameconf->setItemWidget(childitem, 2, frameBox);

                //设置传输缩放下拉框控件
                QComboBox *frameBox1 = new QComboBox;
                for(int y=0;y<10;y++)
                    frameBox1->addItem("1.0E+" + QString::number(y) );
                frameconf->setItemWidget(childitem, 3, frameBox1);
            }
        }else{
            //父节点设置
            QTreeWidgetItem * item=new QTreeWidgetItem(QStringList()<<"FA" );
            frameconf->addTopLevelItem(item);
            //使能选择框设置
            item->setCheckState(1, Qt::Unchecked);
            item->setText(1,  "使能该帧");

            //子节点设置
            for(int x =0 ; x<10 ; x++)
            {
                QTreeWidgetItem * childitem=new QTreeWidgetItem(QStringList()<<"FA");
                item->addChild(childitem);

                childitem->setText(1,QString::number(x+1));

                //设置数据类型下拉框控件
                QComboBox *frameBox = new QComboBox;
                frameBox->addItem("Uint8" );
                frameBox->addItem("Uint16" );
                frameBox->addItem("Int16" );
                frameBox->addItem("Int32" );
                frameBox->addItem("Uint32" );
                frameconf->setItemWidget(childitem, 2, frameBox);

                //设置传输缩放下拉框控件
                QComboBox *frameBox1 = new QComboBox;
                for(int y=0;y<10;y++)
                    frameBox1->addItem("1.0E+" + QString::number(y) );
                frameconf->setItemWidget(childitem, 3, frameBox1);
            }
        }
    }
}
//配置数据容器界面
void userConfView::setDataConfView(QTableWidget* dataconf)
{
    //绘制容器表格
    QStringList tableHeaderList;
    tableHeaderList<<tr("容器")<<tr("用户帧")<<tr("数据位")<<tr("数据值");
    dataconf->setRowCount(20);
    dataconf->setColumnCount(tableHeaderList.size());          //表头列文本设置
    for (int i=0;i<tableHeaderList.size();i++) {
        dataconf->setHorizontalHeaderItem(i,new QTableWidgetItem(tableHeaderList.at(i)));
        dataconf->setColumnWidth(i,dataconf->width());
    }
    dataconf->horizontalHeader()->setStretchLastSection(true);
    dataconf->verticalHeader()->setMinimumSectionSize(35);
    dataconf->resizeRowsToContents();
    dataconf->setSelectionBehavior(QAbstractItemView::SelectRows);//行选中
    dataconf->setShowGrid(false);//不显示网格
    dataconf->setAlternatingRowColors(true);
    dataconf->setFocusPolicy(Qt::NoFocus); //去除选中虚线框
    dataconf->verticalHeader()->setVisible(false); //设置垂直头不可见
    //   dataconf->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    dataconf->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能编写
    dataconf->setSelectionMode(QAbstractItemView::SingleSelection);//只能单次选中
    //容器名
    for(int x=0 , y=0 ; x<20 ; x++)
    {
        QString num = QString::number(x+1);
        dataconf->setItem(x,y, new QTableWidgetItem("userData"+num));
        dataconf->item(x,y)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    //用户帧下拉框设置
    for(int i =0   ; i<20 ; i++)
    {    //用户帧下拉框内容
        QComboBox *frameBox = new QComboBox;
        for(int i =1   ; i<=9 ; i++)
        {
            frameBox->addItem("F"+QString::number(i) );
            if(i==9)
                frameBox->addItem("FA"  );
        }
        //放在哪个地方
        dataconf->setCellWidget(i,1,frameBox);
        //默认选择
        frameBox->setCurrentText("F1");
    }

    //数据位下拉框位置
    for(int i =0   ; i<20 ; i++)
    {    //数据位下拉框内容
        QComboBox *dataLenBox = new QComboBox;
        for(int i =0   ; i<10 ; i++)
        {
            dataLenBox->addItem(QString::number(i+1));
        }
        dataconf->setCellWidget(i,2,dataLenBox);
        //默认选择
        dataLenBox->setCurrentText("1");
    }

    //默认数据值
    for(int i =0   ; i<20 ; i++)
    {
        dataconf->setItem(i,3, new QTableWidgetItem("0"));
    }
}
//读取自定义数据帧配置
void userConfView::readUserConf()
{
    qDebug()<<"readUserConf()";
    //获取用户自定义帧配置窗口句柄
    static QTreeWidget* pQTreeWidget = this->widget()->findChild<QTreeWidget*>();
    //保存用户自定义帧数据类型
    for(int x=0  ; x<10 ; x++){
        //判断此帧是否为选中启用状态
        userConf[x].check_sta = pQTreeWidget->topLevelItem(x)->checkState(1);
        //        qDebug("check_sta = %d",userConf[x].check_sta);
        //保存一帧配置
        for(int y=0 ; y<10 ; y++ ){
            //找到节枝点
            QTreeWidgetItem* pQTreeWidgetItem = pQTreeWidget->topLevelItem(x)->child(y);
            //读取数据类型下拉框
            QString udt=dynamic_cast<QComboBox *>(pQTreeWidget->itemWidget(pQTreeWidgetItem, 2))->currentText();
            if(udt == "Uint8"){
                userConf[x].bit[y].udt = Uint8; //保存数据类型
            }else if(udt == "Uint16"){
                userConf[x].bit[y].udt = Uint16;
            }else if(udt == "Int16"){
                userConf[x].bit[y].udt = Int16;
            }else if(udt == "Int32"){
                userConf[x].bit[y].udt = Int32;
            }else if(udt == "Uint32"){
                userConf[x].bit[y].udt = Uint32;
            }

            //读取传输缩放下拉框
            QString ts=dynamic_cast<QComboBox *>(pQTreeWidget->itemWidget(pQTreeWidgetItem, 3))->currentText();
            if(ts == "1.0E+0"){
                userConf[x].bit[y].ts = M1; //保存传输缩放
            }else if(ts == "1.0E+1"){
                userConf[x].bit[y].ts = M2;
            }else if(ts == "1.0E+2"){
                userConf[x].bit[y].ts = M3;
            }else if(ts == "1.0E+3"){
                userConf[x].bit[y].ts = M4;
            }else if(ts == "1.0E+4"){
                userConf[x].bit[y].ts = M5;
            }else if(ts == "1.0E+5"){
                userConf[x].bit[y].ts = M6;
            }else if(ts == "1.0E+6"){
                userConf[x].bit[y].ts = M7;
            }else if(ts == "1.0E+7"){
                userConf[x].bit[y].ts = M8;
            }else if(ts == "1.0E+8"){
                userConf[x].bit[y].ts = M9;
            }else if(ts == "1.0E+9"){
                userConf[x].bit[y].ts = M10;
            }
            //            qDebug("udt = %s    ,  ts = %s",qPrintable( udt ) ,qPrintable( ts ));
            //            qDebug("x = %d    ,  y = %d",x ,y);
        }

    }
}
//读取数据容器配置
void userConfView::readDataConf()
{
    qDebug()<<"readDataConf()";
    /*表格中下拉框获取选中值*/
    //QString strValue=dynamic_cast<QComboBox *>(ui->tableWidget->cellWidget(0,2))->currentText();

    //获取数据容器配置窗口句柄
    static QTableWidget* pQTableWidget = this->widget()->findChild<QTableWidget*>();

    //保存数据容器对应哪个帧
    for(int i = 0 ; i< 20 ; i++){
        QString strValue=dynamic_cast<QComboBox *>(pQTableWidget->cellWidget(i,1))->currentText();
        if(strValue == QString("F1")){
            dataConf[i].frameNum = F1;
        }else if(strValue == QString("F2")){
            dataConf[i].frameNum = F2;
        }else if(strValue == QString("F3")){
            dataConf[i].frameNum = F3;
        }else if(strValue == QString("F4")){
            dataConf[i].frameNum = F4;
        }else if(strValue == QString("F5")){
            dataConf[i].frameNum = F5;
        }else if(strValue == QString("F6")){
            dataConf[i].frameNum = F6;
        }else if(strValue == QString("F7")){
            dataConf[i].frameNum = F7;
        }else if(strValue == QString("F8")){
            dataConf[i].frameNum = F8;
        }else if(strValue == QString("F9")){
            dataConf[i].frameNum = F9;
        }else if(strValue == QString("FA")){
            dataConf[i].frameNum = FA;
        }
    }

    //保存数据容器对应哪个数据位
    for(int i = 0 ; i< 20 ; i++){
        QString strValue=dynamic_cast<QComboBox *>(pQTableWidget->cellWidget(i,2))->currentText();
        if(strValue == QString("1")){
            dataConf[i].bitNum = B1;
        }else if(strValue == QString("2")){
            dataConf[i].bitNum = B2;
        }else if(strValue == QString("3")){
            dataConf[i].bitNum = B3;
        }else if(strValue == QString("4")){
            dataConf[i].bitNum = B4;
        }else if(strValue == QString("5")){
            dataConf[i].bitNum = B5;
        }else if(strValue == QString("6")){
            dataConf[i].bitNum = B6;
        }else if(strValue == QString("7")){
            dataConf[i].bitNum = B7;
        }else if(strValue == QString("8")){
            dataConf[i].bitNum = B8;
        }else if(strValue == QString("9")){
            dataConf[i].bitNum = B9;
        }else if(strValue == QString("10")){
            dataConf[i].bitNum = B10;
        }
    }

}

//用户自定义帧解析方法组合数据
void userConfView::combinationUserData(void *mframe)
{
    qDebug()<<"combinationUserData()";

    //定义字节组合函数
    auto getUInt8  =  [](unsigned char *pData, unsigned int *i){
        unsigned  char result = 0;
        result = (unsigned  char)pData[*i];
        *i += 1 ;
        return result ;
    };
    auto getUInt16 =  [](unsigned char *pData, unsigned int *i){
        unsigned  short result = 0;
        result = (unsigned  short)pData[*i];
        *i += 1 ;
        result += (unsigned  short)(pData[*i]<<8);
        *i += 1 ;
        return result ;
    };
    auto getInt16  =  [](unsigned char* pData, unsigned int* i){
        short result = 0;
        result = (short)pData[*i];
        *i += 1 ;
        result += (short)(pData[*i]<<8);
        *i += 1 ;
        return result ;
    };
    auto getUInt32 =  [](unsigned char *pData, unsigned int *i){
        unsigned  int result = 0;
        result = (int)pData[*i];
        *i += 1 ;
        result += (int)(pData[*i]<<8);
        *i += 1 ;
        result += (int)(pData[*i]<<16);
        *i += 1 ;
        result += (int)(pData[*i]<<24);
        *i += 1 ;
        return result ;
    };
    auto getInt32  =  [](unsigned char *pData, unsigned int *i){
        int result = 0;
        result = (int)pData[*i];
        *i += 1 ;
        result += (int)(pData[*i]<<8);
        *i += 1 ;
        result += (int)(pData[*i]<<16);
        *i += 1 ;
        result += (int)(pData[*i]<<24);
        *i += 1 ;
        return result ;
    };
    //是否为用户自定义数据帧?
//    if(mframe.Id()<0xF0){
//        return ;
//    }

    //保存帧描述
    uesrFinalFrame.head=0;
    uesrFinalFrame.addr=0;
    uesrFinalFrame.id=0;

    uesrFinalFrame.ac=0;
    uesrFinalFrame.sc=0;

    int lenindex = 0;
    unsigned char* pBuf ;

//    uesrFinalFrame.head=mframe.Head();
//    uesrFinalFrame.addr=mframe.Addr();
//    uesrFinalFrame.id=mframe.Id();

//    uesrFinalFrame.ac=mframe.Ac();
//    uesrFinalFrame.sc=mframe.Sc();

//    int lenindex = mframe.Len() ;
//    unsigned char* pBuf = mframe.DataPoint() ;
    //根据配置解析数据
    unsigned char idindex = uesrFinalFrame.id-0xF1; //对应的功能帧索引
    qDebug("idindex res:%d", idindex);
    unsigned int bufIndex = 0 ;

    for(int i = 0 ; i<10 ; i++){
        //数据归零
        uesrFinalFrame.data[i].Uint32 = 0;
    }
    //读取配置数据
    readUserConf();
    readDataConf();
    //根据配置的数据类型组合数据
    for (uesrFinalFrame.len=0; uesrFinalFrame.len<10 && lenindex > 0 ; uesrFinalFrame.len++) {
        switch (userConf[idindex].bit[uesrFinalFrame.len].udt) {
        case Uint8 :
            lenindex-=1; //读取一字节
            uesrFinalFrame.data[uesrFinalFrame.len].Uint8  =  getUInt8( pBuf , &bufIndex) ;
            qDebug("Uint8 res:%d", uesrFinalFrame.data[uesrFinalFrame.len].Uint8);
            break;
        case Int16 :
            lenindex-=2; //读取二字节
            uesrFinalFrame.data[uesrFinalFrame.len].Int16  =  getInt16( pBuf , &bufIndex) ;
            qDebug("Int16 res:%d", uesrFinalFrame.data[uesrFinalFrame.len].Int16);
            break;
        case Uint16 :
            lenindex-=2; //读取二字节
            uesrFinalFrame.data[uesrFinalFrame.len].Uint16 =  getUInt16( pBuf , &bufIndex) ;
            qDebug("Uint16 res:%d", uesrFinalFrame.data[uesrFinalFrame.len].Uint16);
            break;
        case Int32  :
            lenindex-=4; //读取四字节
            uesrFinalFrame.data[uesrFinalFrame.len].Int32  =  getInt32( pBuf , &bufIndex) ;
            qDebug("Int32 res:%d", uesrFinalFrame.data[uesrFinalFrame.len].Int32);
            break;
        case Uint32 :
            lenindex-=4; //读取四字节
            uesrFinalFrame.data[uesrFinalFrame.len].Uint32  =  getUInt32( pBuf , &bufIndex) ;
            qDebug("Uint32 res:%d", uesrFinalFrame.data[uesrFinalFrame.len].Uint32);
            break;
        }
    }

    //最终解析帧转成通过信号发送到tablewifget显示
    emit showframe(&uesrFinalFrame, 0);
    //刷新UI显示值
    refreshView();

}


//刷新UI显示值
void userConfView::refreshView()
{
    static long int frameCount = 0;//统计接收的总数据包数(由不同的帧组合)
    frameCount++;
    //------------------------------------------------------------------------------------
    //获取数据容器配置窗口句柄
    static QTableWidget* pQTableWidget = this->widget()->findChild<QTableWidget*>();

    //遍历数据容器
    for(int i = 0 ; i<20 ;i++){
        //遍历用户帧配置
        for(int x=0 ; x<10 ; x++){
            //判断当前帧的id与要显示的帧id是否相符                    //如果用户帧使能
            if(uesrFinalFrame.id-0xF1 == dataConf[i].frameNum && userConf[x].check_sta==2){
                //读取哪个数据位
                int dataNum = dataConf[i].bitNum;
                //缩放比例
                int howBig = pow(10,userConf[x].bit[dataNum].ts);
                //根据缩放比例换算结果
                double res = static_cast<double>(uesrFinalFrame.data[dataNum].Uint32)/howBig;
                //刷新表格数据
                pQTableWidget->item(i,3)->setText(QString::number(res));
                //刷新数据面板用户自定义数据显示
                emit refreshDataView(i,res);
                //发送显示数据波形
                emit refreshPrint(frameCount , res , i );

                break;
            }
        }
    }


    //------------------------------------------------------------------------------------


}


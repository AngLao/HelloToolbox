#include "frmdevice.h"
#include "ui_frmdevice.h"
#include "qfontdatabase.h"

frmDevice::frmDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDevice)
{
    ui->setupUi(this);

    //引入图形字体
    //    int fontId = QFontDatabase::addApplicationFont(":/image/fontawesome-webfont.ttf");
    //    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    //    QFont iconFont = QFont(fontName);
    //    iconFont.setPixelSize(15);

    //    ui->labDevice->setFont(iconFont);
    //   ui->labDevice->setText(QChar(0xf06c));
    //
    //    ui->labTemp->setFont(iconFont);
    //   ui->labTemp->setText(QChar(0xf0eb));

    //    ui->labDamp->setFont(iconFont);
    //    ui->labDamp->setText(QChar(0xf043));
}

frmDevice::~frmDevice()
{
    delete ui;
}

void frmDevice::addFrmDevice(QStringList* mQStringlis)
{
    if(mQStringlis->size()>0)
    {
        mQStringlis->constData();

        //顶部标签
        setTop( mQStringlis->at(0) );

        //根据需求创建数据标签
        for(int i=1; i < mQStringlis->size()-1 ; i++)
        {
            QLabel *Label = new QLabel(this) ;
            Label->setText(mQStringlis->at(i));

            ui->verticalLayout_2->addWidget(Label);

        }
        //分割线
        //-----------------------
        //底部标签
        setBottom( mQStringlis->at(mQStringlis->size()-1) );
    }

}

void frmDevice::setTop(QString pQString)
{

    QList<QLabel*> pLabelChildren1 =  this->findChildren<QLabel*>();

    //顶部标签是零,底部标签是一
    pLabelChildren1.at(0)->setText(pQString);
    //对齐方式
    ui->labDeviceName->setAlignment(Qt::AlignCenter);

}

void frmDevice::setBottom(QString pQString)
{

    QList<QLabel*> pLabelChildren1 =  this->findChildren<QLabel*>();
    //顶部标签是零,底部标签是一
    pLabelChildren1.at(1)->setText(pQString);

}

void frmDevice::setLabelValue( QStringList *QStringList)
{
    QList<QLabel*> pLabelChildren1 =  this->findChildren<QLabel*>();

    //新建标签是按顺序加入,及从二开始
    for(int i=2 ; i < pLabelChildren1.size() ; i++)
    {

        qDebug("asd:%lld",pLabelChildren1.size());
        pLabelChildren1.at(i)->setText(QStringList->at(i-2));
    }
}


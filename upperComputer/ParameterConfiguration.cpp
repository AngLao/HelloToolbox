#include "ParameterConfiguration.h"

ParameterConfiguration::ParameterConfiguration(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* ctrLayout = new QVBoxLayout();
    QList< QWidget*> ctrblockList ;
    for (int var = 0; var < 6; ++var) {

        ctrblockList.append(addCtrBlock());
        ctrLayout->addWidget(ctrblockList.at(var));
    }

    QWidget* confBlock = addConfBlock();
    //主布局设置
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(2,0,2,0);
    mainLayout->setSpacing(2);
    mainLayout->addWidget(confBlock,2);
    mainLayout->addLayout(ctrLayout,8);
    debugViewWidget = new QWidget();
    debugViewWidget->setLayout(mainLayout);

}


QWidget* ParameterConfiguration::widget()
{
    return debugViewWidget;
}

QWidget* ParameterConfiguration::addCtrBlock()
{
    //显示名称控件配置
    QLabel *labname = new QLabel("地址ID:");
    QLabel *lab_set = new QLabel("值:");
    QLabel *lab_zoom = new QLabel("缩放:");
    QLabel *lab_step = new QLabel("步进:");
    QLabel *lab_min = new QLabel("min:");
    QLabel *lab_max = new QLabel("max:");

    //数值编辑控件配置
    QSpinBox *sb_set = new QSpinBox();  //设置数值
    QLineEdit *le_name = new QLineEdit("00");//目标地址和id
    QLineEdit *le_zoom = new QLineEdit(QString::number(1));//传输缩放
    QLineEdit *le_step = new QLineEdit(QString::number(1));//微调步进
    QLineEdit *le_min = new QLineEdit(QString::number(0));//可传输最大值
    QLineEdit *le_max = new QLineEdit(QString::number(500));//可传输最小值

    //带符号32位数值,规范QLineEdit输入格式
    le_name->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+")));
    le_zoom->setValidator(new QIntValidator(-500, 500));
    le_step->setValidator(new QIntValidator(-500, 500));
    le_min->setValidator(new QIntValidator(-2147483647, 2147483647));
    le_max->setValidator(new QIntValidator(-2147483647, 2147483647));


    sb_set->setMinimum(le_min->text().toInt());
    sb_set->setMaximum(le_max->text().toInt());
    sb_set->setSingleStep(le_step->text().toInt());
    sb_set->setValue(0);

    //滑杆控件配置
    QSlider *hs_set = new QSlider();
    hs_set->setOrientation(Qt::Horizontal);
    hs_set->setMinimum(le_min->text().toInt());
    hs_set->setMaximum(le_max->text().toInt());
    hs_set->setValue(0);

    //----------槽函数链接----------
    //设定值改变时响应 打包数据串口发送命令
    connect(sb_set, &QSpinBox::valueChanged,this,[=](){
        hs_set->setValue(sb_set->value());
        //发送数据
        auto text = le_name->text().toUtf8();
        uint32_t data = sb_set->text().toUInt();

        static frame_t senbuf;
        easy_set_header(&senbuf, TargetHeader);
        easy_set_address(&senbuf, int(text.at(0)));
        easy_set_id(&senbuf, int(text.at(1)));

        easy_wipe_data(&senbuf);
        easy_add_data(&senbuf, data, 4);
        easy_add_check(&senbuf);
        //发送帧数据
        uint8_t len = easy_return_buflen(&senbuf);

        emit sendPackData((char*)&senbuf ,len);
    });

    //滑杆操作响应
    connect(hs_set, &QSlider::sliderReleased,this,[=](){
        sb_set->setValue(hs_set->value());
    });

    //地址ID改变时响应
    connect(le_name, &QLineEdit::editingFinished,this,[=](){
        auto text = le_name->text() ;
        if(text.length()!=2)
            le_name->setText("00");
    });

    //步进值改变时响应
    connect(le_step, &QLineEdit::editingFinished,this,[=](){
        sb_set->setSingleStep(le_step->text().toInt());
    });

    //最大最小值编辑结束时响应
    connect(le_max, &QLineEdit::editingFinished,this,[=](){
        int changvalue = le_max->text().toInt();
        int thisvalue = sb_set->value();
        sb_set->setMaximum(changvalue);
        hs_set->setMaximum(changvalue);
        sb_set->setValue(thisvalue);
        hs_set->setValue(thisvalue);

    });
    connect(le_min, &QLineEdit::editingFinished,this,[=](){
        int changvalue = le_min->text().toInt();
        int thisvalue = sb_set->value();
        sb_set->setMinimum(changvalue);
        hs_set->setMinimum(changvalue);
        sb_set->setValue(thisvalue);
        hs_set->setValue(thisvalue);

    });


    //开始布局
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(labname);
    topLayout->addWidget(le_name,1);
    topLayout->addWidget(lab_set);
    topLayout->addWidget(sb_set,1);
    topLayout->addWidget(lab_zoom);
    topLayout->addWidget(le_zoom,1);
    topLayout->addWidget(lab_step);
    topLayout->addWidget(le_step,1);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(hs_set,6);
    bottomLayout->addWidget(lab_min);
    bottomLayout->addWidget(le_min,1);
    bottomLayout->addWidget(lab_max);
    bottomLayout->addWidget(le_max,1);

    //主布局设置
    QFrame *mainFrame = new QFrame();
    QVBoxLayout* mainLayout = new QVBoxLayout(mainFrame);
    mainLayout->setContentsMargins(2,0,2,0);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    return mainFrame;
}

QWidget* ParameterConfiguration::addConfBlock()
{
    QLabel *lab_header = new QLabel("目标帧头:");
    QLabel *lab_checkmodel = new QLabel("校验模式:");

    QLineEdit *le_header = new QLineEdit(QString::number(REC_HEADER));
    //只能是一个字节
    le_header->setValidator(new QIntValidator(0, 255));
    TargetHeader = REC_HEADER;
    connect(le_header,&QLineEdit::editingFinished, this, [=]{
        TargetHeader = le_header->text().toInt();
    });
    QLineEdit *le_checkmodel = new QLineEdit(QString::number(check_model));
    le_checkmodel->setDisabled(true);



    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(lab_header);
    headerLayout->addWidget(le_header);
    QHBoxLayout *checkmodelLayout = new QHBoxLayout();
    checkmodelLayout->addWidget(lab_checkmodel);
    checkmodelLayout->addWidget(le_checkmodel);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QPushButton *readConfButton = new QPushButton();
    readConfButton->setSizePolicy(sizePolicy);
    readConfButton->setText("读取配置");
    QPushButton *saveConfButton = new QPushButton();
    saveConfButton->setSizePolicy(sizePolicy);
    saveConfButton->setText("保存配置");
    QPushButton *sendConfButton = new QPushButton();
    sendConfButton->setSizePolicy(sizePolicy);
    sendConfButton->setText("全部发送");

    //功能未完善
    readConfButton->setDisabled(true);
    saveConfButton->setDisabled(true);
    sendConfButton->setDisabled(true);
    //主布局设置
    QFrame *mainFrame = new QFrame();
    QVBoxLayout* mainLayout = new QVBoxLayout(mainFrame);
    mainLayout->setContentsMargins(2,2,2,2);
    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(checkmodelLayout);
    mainLayout->addWidget(readConfButton);
    mainLayout->addWidget(saveConfButton);
    mainLayout->addWidget(sendConfButton);


    return mainFrame;
}

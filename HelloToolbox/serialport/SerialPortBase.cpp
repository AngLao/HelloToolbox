#include "SerialPortBase.h"

SerialPortBase::SerialPortBase(void)
{
    ui.setupUi(&widget);

    //发送输入框消息按钮信号和槽
    connect(&serialport, &QSerialPort::errorOccurred, this, [&](QSerialPort::SerialPortError error){
        if(error == QSerialPort::PermissionError){
            ui.RxDataTextEdit->append("串口连接中断");
            OpenSerialPort();
            FineSerialPort();
        }
        qDebug()<<error;
    });

    //使选择下拉框具备点击响应,点击时进行一次搜寻串口操作
    ui.SerialPortChooseComboBox->installEventFilter(this);

    //连接打开按钮按钮信号和槽
    connect(ui.OpenSerialPortPushButton, &QPushButton::clicked, this, &SerialPortBase::OpenSerialPort );

    //清除接收框消息按钮信号和槽
    connect(ui.RxDataTextClearPushButton, &QPushButton::clicked, this, [&](){
        ui.RxDataTextEdit->clear();
    });

    //清除发送框消息按钮信号和槽
    connect(ui.SendDataTextClearPushButton, &QPushButton::clicked, this, [&](){
        ui.TxDataTextEdit->clear();
    });

    //发送输入框消息按钮信号和槽
    connect(ui.SendDataPushButton, &QPushButton::clicked, this, [&](){
        QByteArray GetHexShowArray = QByteArray(ui.TxDataTextEdit->toPlainText().toLocal8Bit());

        /*发送16进制文本*/
        if(ui.SendDataForHexCheckBox->isChecked())
            GetHexShowArray = QByteArray::fromHex(GetHexShowArray);

        /*发送回车*/
        if(ui.SendDataWithEnterCheckBox->isChecked())
            GetHexShowArray.append("\r\n");

        SendData(GetHexShowArray,GetHexShowArray.size());
    });

    //定时发送消息
    sendTimer.setTimerType(Qt::PreciseTimer);
    connect(&sendTimer, &QTimer::timeout,this,[&](){
        //模拟点击发送按钮
        ui.SendDataPushButton->click();
    });
    connect(ui.SendDataPeriodCheckBox,&QCheckBox::stateChanged,this,[&](){
        if(ui.SendDataPeriodLineEdit->text().isEmpty())
        {
            /*设置的发送时间为空，不启动发送*/
            ui.SendDataPeriodCheckBox->setCheckState(Qt::Unchecked);
            QMessageBox::information(this,"提示","请检查发送时间设置是否正确！");
        }
        //打开定时器发送选择框
        if(ui.SendDataPeriodCheckBox->isChecked() &&
            ui.SendDataPeriodCheckBox->isChecked()){
            sendTimer.setInterval(ui.SendDataPeriodLineEdit->text().toInt());
            sendTimer.start();
        }
        else
            sendTimer.stop();
    });

    RefreshCountTimerInit();

    //打开软件先搜索一次存在的串口
    FineSerialPort();
}

void SerialPortBase::FineSerialPort(void){
    QStringList SerialPortNameList;/*保存搜索到的串口，存入列表中*/
//    ui.RxDataTextEdit->clear();
    ui.RxDataTextEdit->append("存在的串口：");
    foreach (const QSerialPortInfo &SerialPortInfo, QSerialPortInfo::availablePorts()) /*遍历可使用的串口*/
    {
        SerialPortNameList.append(SerialPortInfo.portName());/*把搜索到的串口存入列表中*/
        ui.RxDataTextEdit->append(SerialPortInfo.portName() + " " + SerialPortInfo.description());
    }
    ui.SerialPortChooseComboBox->clear();
    ui.SerialPortChooseComboBox->addItems(SerialPortNameList);/*将搜索到的串口显示到UI界面上*/
//    qDebug()<<"find serial";
};

void SerialPortBase::RefreshCountTimerInit(void){
    //收发计数标签刷新显示
    static QTimer timer;

    connect(&timer, &QTimer::timeout,this,[&](){
        QString text = "接收计数:" + QString::number(recCount);
        ui.ReceiveCountLabel->setText(text);
        text = "发送计数:" + QString::number(sendCount);
        ui.SendCountLabel->setText(text);
    });

    timer.setInterval(50);
    timer.setTimerType(Qt::VeryCoarseTimer);
    timer.start();

    //清除收发计数按钮信号和槽
    connect(ui.CountClearButton, &QPushButton::clicked, this, [&](){
        recCount = 0;
        sendCount = 0;
    });
}

void SerialPortBase::SendData(const char *data , const int DataLen =1)
{
    sendCount+=DataLen;
    /*发送数据*/
    if(serialport.isOpen())
        serialport.write(data,DataLen);
    else
        qDebug()<<"SerialPort error";
}

void SerialPortBase::OpenSerialPort()
{
    if(ui.OpenSerialPortPushButton->text() == "打开串口")
    {
        /*设置选中的COM口*/
        serialport.setPortName(ui.SerialPortChooseComboBox->currentText());

        /*设置串口的波特率*/
        bool res = serialport.setBaudRate(ui.BaudRateComboBox->currentText().toInt());
        if(res == false){
            ui.RxDataTextEdit->append("波特率设置失败");
            return;
        }
        /*设置数据位数*/
        serialport.setDataBits( QSerialPort::DataBits(ui.PortDataBitsComboBox->currentText().toInt()) );

        /*设置奇偶校验,NoParit无校验*/
        int index = ui.PortParityComboBox->currentIndex();
        QSerialPort::Parity PortParityBits = (index == 0) ? (QSerialPort::NoParity) :QSerialPort::Parity(index+1);
        serialport.setParity(PortParityBits);

        /*设置停止位，OneStop一个停止位*/
        serialport.setStopBits( QSerialPort::StopBits((ui.PortStopBitsComboBox->currentIndex()+1)) );

        /*设置流控制，NoFlowControl无流控制*/
        serialport.setFlowControl( QSerialPort::NoFlowControl );

        /*ReadWrite设置的是可读可写的属性*/
        if(serialport.open(QIODevice::ReadWrite) == true){
            ui.RxDataTextEdit->append(ui.SerialPortChooseComboBox->currentText() + "已连接");

            ui.SendDataPushButton->setEnabled(true);/*使能串口发送按键*/
            ui.SendDataPeriodCheckBox->setEnabled(true);  /*打开串口后可以设置自动发送*/

            ui.BaudRateComboBox->setEnabled(false);
            ui.PortStopBitsComboBox->setEnabled(false);
            ui.PortDataBitsComboBox->setEnabled(false);
            ui.PortParityComboBox->setEnabled(false);/*连接成功后设置ComboBox不可选择*/
            ui.SerialPortChooseComboBox->setEnabled(false);/*列表框无效*/

            /*打开串口成功，连接信号和槽*/
            connect(&serialport,&QSerialPort::readyRead,this,&SerialPortBase::DataPreprocessing);
            ui.OpenSerialPortPushButton->setText("关闭串口");
        }else{
            ui.RxDataTextEdit->append(ui.SerialPortChooseComboBox->currentText() + "连接失败");
        }
    }else{

        serialport.close();/*关闭串口*/
        disconnect(&serialport,&QSerialPort::readyRead,this,&SerialPortBase::DataPreprocessing);

        ui.RxDataTextEdit->append(ui.SerialPortChooseComboBox->currentText() + "已关闭");

        ui.SerialPortChooseComboBox->setEnabled(true);/*列表框有效*/
        ui.OpenSerialPortPushButton->setText("打开串口");

        ui.SendDataPushButton->setEnabled(false);/*失能串口发送按键*/
        ui.SendDataPeriodCheckBox->setEnabled(false);  /*失能自动发送按键*/

        ui.BaudRateComboBox->setEnabled(true);
        ui.PortStopBitsComboBox->setEnabled(true);
        ui.PortDataBitsComboBox->setEnabled(true);
        ui.PortParityComboBox->setEnabled(true);/*关闭连接后设置ComboBox选择*/

    }
}

void SerialPortBase::DataPreprocessing()
{
    QByteArray SerialPortDataBuf = serialport.readAll();

    //刷新接收计数
    recCount+=SerialPortDataBuf.size();

    /*显示时间戳*/
    if(ui.RxDataWithTimeCheckBox->isChecked())
        ui.RxDataTextEdit->append("["+QDateTime::currentDateTime().toString("hh:mm:ss:zzz")+"]:");

    /*16进制显示*/
    if(ui.RxDataForHexCheckBox->isChecked()){
        QTextCursor cursorPosition = ui.RxDataTextEdit->textCursor();
        cursorPosition.insertText(SerialPortDataBuf.toHex(' ').toUpper());
    }
    else{
        QTextCursor cursorPosition = ui.RxDataTextEdit->textCursor();
        cursorPosition.insertText(SerialPortDataBuf);
    }
    //解析是否存在mavlink协议数据
    foreach(uint8_t byte, SerialPortDataBuf)
    {
        if (mavlink_parse_char(chan, byte, &msg, &status) == 0)
            continue;

        switch(msg.msgid){
        case MAVLINK_MSG_ID_RAW_IMU:
            qDebug("msg id : RAW_IMU\n");
            mavlink_msg_raw_imu_decode(&msg, &raw_imu_data);

            break;
        case MAVLINK_MSG_ID_ATTITUDE:
            qDebug("msg id : ATTITUDE\n");
            mavlink_msg_attitude_decode(&msg, &imu_data);

            emit gyroDataupdate(imu_data.pitchspeed,imu_data.rollspeed,imu_data.yawspeed);
            break;
        }

    }

    //保持编辑器光标在最后一行
    ui.RxDataTextEdit->moveCursor(QTextCursor::End);

}

bool SerialPortBase::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
        if(obj == ui.SerialPortChooseComboBox)
            FineSerialPort();

    return QWidget::eventFilter(obj, event);
}

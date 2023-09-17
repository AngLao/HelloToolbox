#ifndef SERIALPORTBASE_H
#define SERIALPORTBASE_H

#pragma once

#include <QWidget>
#include <QMouseEvent>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QComboBox>
#include <QMouseEvent>
#include <QTextEdit>

#include <mavlink/V2.0/common/mavlink.h>

class SerialPortBase: public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortBase(void);
    ~SerialPortBase(){};


    class Ui_SerialPortBase* ui;
    QWidget* Widget(){return pWidget;};
    void SendData(const char *data , const int DataLen );
private:
    QSerialPort *pSerialPort;
    QWidget *pWidget;
    long long int recCount = 0;
    long long int sendCount = 0;

    mavlink_system_t mavlink_system = {
        1, // System ID (1-255)
        1  // Component ID (a MAV_COMPONENT value)
    };
    mavlink_status_t status;
    mavlink_message_t msg;
    int chan = MAVLINK_COMM_0;

    void RefreshCountTimerInit(void);
    void OpenSerialPort();
    void DataPreprocessing();

signals:

};

#endif // SERIALPORTBASE_H

#ifndef SERIALPORTBASE_H
#define SERIALPORTBASE_H

#pragma once
#include <QObject>
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

#include "ui_SerialPortBase.h"
#include "../mavlink/V2.0/common/mavlink.h"

class SerialPortBase: public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortBase(void);
    ~SerialPortBase(){};

    QWidget* Widget(){return &widget;};
    void SendData(const char *data , const int DataLen );
    bool eventFilter(QObject *, QEvent *);
private:
    void FineSerialPort();
    void RefreshCountTimerInit(void);
    void OpenSerialPort();
    void extracted(QByteArray &SerialPortDataBuf);
    void DataPreprocessing();

private:
    Ui_SerialPortBase ui;
    QSerialPort serialport;
    QWidget widget;
    QTimer sendTimer;

    long long int recCount = 0;
    long long int sendCount = 0;

    mavlink_system_t mavlink_system = {
        1, // System ID (1-255)
        1  // Component ID (a MAV_COMPONENT value)
    };
    mavlink_status_t status;
    mavlink_message_t msg;
    int chan = MAVLINK_COMM_0;

    mavlink_raw_imu_t   raw_imu_data;
    mavlink_attitude_t  imu_data;
signals:
    void gyroDataupdate(float gyroX, float gyroY, float gyroZ);
};

#endif // SERIALPORTBASE_H

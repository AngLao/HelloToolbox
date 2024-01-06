#ifndef __NETWORK_H__
#define __NETWORK_H__

#pragma once
#include <QObject>
#include <QWidget>

#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QTextEdit>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QThread>
#include <QFile>

class NetworkManage: public QObject
{
    Q_OBJECT
public:
    explicit NetworkManage(void);
    ~NetworkManage(){};

    QWidget* Widget(){return pWidget;};
    void ReceiveData();
private:
    int StartClient();
    void StopClient();
    void SendData();

public:
private:
    QWidget *pWidget;
    class Ui_network *ui;

    QTimer *countTimer;
    unsigned long sendCount, receiveCount;
    QUdpSocket *udpSocket;
    QHostAddress serverAddress;
    quint16 serverPort;

signals:

};




#endif

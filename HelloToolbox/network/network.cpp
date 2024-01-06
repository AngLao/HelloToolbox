#include "network.h"
#include "ui_network.h"

NetworkManage::NetworkManage()
{
    pWidget = new QWidget();
    ui = new class Ui_network();
    ui->setupUi(pWidget);

    //读取服务器配置信息
    QFile file("../info.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);  //用文件构造流
        ui->udp_client_ip->setText(in.readLine());
        ui->udp_client_port->setText(in.readLine());
    }
    file.close();

    QObject::connect(ui->udp_connect, &QPushButton::clicked, this, [&](){
        if(ui->udp_connect->text() == "启动客户端"){
            qDebug()<<"启动客户端";
            if(StartClient() != -1)
                ui->udp_connect->setText("关闭客户端");
        }else{
            StopClient();
            ui->udp_connect->setText("启动客户端");
        }
    });

    QObject::connect(ui->send, &QPushButton::clicked, this, &NetworkManage::SendData);

    countTimer = new QTimer;
    sendCount = 0;
    receiveCount = 0;
    connect(countTimer, &QTimer::timeout, this, [this](){
        ui->receive_count->setText(QString::number(receiveCount));
        ui->send_count->setText(QString::number(sendCount));
    });
    countTimer->start(50);
}

int NetworkManage::StartClient()
{
    QHostAddress address(ui->udp_client_ip->text());
    quint16 port = ui->udp_client_port->text().toInt();

    if(address.isNull()){
        qDebug()<<"address error";
        return -1;
    }
    if(port>65535 || port<0){
        qDebug()<<"port error";
        return -1;
    }

    serverAddress = address;
    serverPort = port;

    udpSocket = new QUdpSocket;
    connect(udpSocket, &QUdpSocket::readyRead, this, &NetworkManage::ReceiveData);

    ui->send->setEnabled(true);
    ui->status->append("客户端已启动");
    return 0;
}

void NetworkManage::StopClient()
{
    disconnect(udpSocket, &QUdpSocket::readyRead, this, &NetworkManage::ReceiveData);
    udpSocket->abort();
    udpSocket->deleteLater();

    ui->send->setDisabled(true);
    ui->status->append("客户端已关闭");
}

void NetworkManage::SendData()
{
    QByteArray datagram = ui->send_str->text().toLocal8Bit();
    sendCount += datagram.size();
    udpSocket->writeDatagram(datagram, serverAddress, serverPort);
}

void NetworkManage::ReceiveData()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        receiveCount += datagram.size();
        udpSocket->readDatagram(datagram.data(), datagram.size());
//        qDebug() << "Received data:" << datagram;
    }
}


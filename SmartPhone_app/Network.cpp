#include "Network.h"

#include<QNetworkDatagram>
#include<QDebug>

Network::Network()
    : QObject()
{
    initSocket();
}

Network::~Network()
{

}

void Network::initSocket()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(QHostAddress("192.168.0.23"), 7755);
    m_udpSocket->connectToHost("192.168.0.31", 7755,QIODevice::ReadWrite);

    connect(m_udpSocket, &QUdpSocket::readyRead,
            this, &Network::readPendingDatagrams);
}

void Network::readPendingDatagrams()
{
    qDebug() << __PRETTY_FUNCTION__;
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        //processTheDatagram(datagram);
    }
}

void Network::sendMessage(QString msg)
{
    qDebug() << __PRETTY_FUNCTION__;
    auto ret = m_udpSocket->write(msg.toUtf8(),msg.length());
    qDebug() << "write return "<< ret;
}

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
    m_udpSocket->bind(QHostAddress("192.168.0.31"), 7755);

    connect(m_udpSocket, &QUdpSocket::readyRead,
            this, &Network::readPendingDatagrams);
}

void Network::readPendingDatagrams()
{
    qDebug() << __PRETTY_FUNCTION__;
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        qDebug() <<"read "<< datagram.data();
        //processTheDatagram(datagram);
    }
}

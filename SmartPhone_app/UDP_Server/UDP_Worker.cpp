#include "UDP_Worker.h"

#include <QDebug>
#include <QTextCodec>
#include <QThread>
#include <QDataStream>




UDP_Worker::UDP_Worker(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Lancement du serveur UDP..." ;

   m_UdpSocket = new QUdpSocket(this);
   m_UdpSocket->bind(QHostAddress("192.168.0.23"), 7755);
   m_UdpSocket->connectToHost(QHostAddress("192.168.0.31"), 7755);

   connect(m_UdpSocket, &QUdpSocket::readyRead,
           this, &UDP_Worker::readPendingDatagrams);


}

void UDP_Worker::readPendingDatagrams()
{
   qDebug() << __PRETTY_FUNCTION__;
   while (m_UdpSocket->hasPendingDatagrams()) {
       QNetworkDatagram datagram = m_UdpSocket->receiveDatagram();
       //qDebug() <<"read "<< datagram.data()<<" from "<<datagram.senderAddress()<<":"<<datagram.senderPort();

       processTheDatagram(datagram);
   }
}


void UDP_Worker::process()
{


}

void UDP_Worker::processTheDatagram(QNetworkDatagram datagram)
{
    qDebug() << __PRETTY_FUNCTION__;

    qDebug() << "datagram.data().size() = "<<datagram.data().size();
    if( datagram.data() == QByteArray("INIT_CONNECTION"))
    {
        m_UdpSocket->connectToHost(datagram.senderAddress(), datagram.senderPort(),QIODevice::ReadWrite);
        emit connected();
    }
    else if(datagram.data() == QByteArray("CLOSE_CONNECTION"))
    {

    }
    else if(datagram.data().size() > 4 &&
            datagram.data().at(0) == (char)0xFF &&
            datagram.data().at(1) == (char)0x01)
    {
        qDebug() << "IMAGE";
        qDebug() << "datagram.data().size() = "<<datagram.data().size();
        QByteArray byteArrayImage = datagram.data();
        QDataStream streamImage(&byteArrayImage, QIODevice::ReadOnly);

        uint8_t tmp;

        streamImage >> tmp;
        streamImage >> tmp;
        streamImage >> width;
        streamImage >> height;
//        m_dataVector.resize(width*height*3);
//        streamImage.readBytes(m_data.data(),width*height*3);

        qDebug() <<"width = "<<width;
        qDebug() <<"height = "<<height;

    }
    else
    {
        QByteArray byteArrayImage = datagram.data();
        QDataStream streamImage(&byteArrayImage, QIODevice::ReadOnly);
        m_data = new char[width*height*3];
        int imagelenght = width*height*3;
        streamImage.readRawData(m_data,imagelenght);
        m_image = QImage((uchar*)m_data,width,height,QImage::Format_RGB888);
        bool convertFromImage = m_pixmap.convertFromImage(m_image);
        if(convertFromImage)
            emit newImage(m_pixmap);


    }


}

void UDP_Worker::sendMessage(QString msg)
{
    qDebug() << __PRETTY_FUNCTION__;
    auto ret = m_UdpSocket->write(msg.toUtf8(),msg.length());
    qDebug() << "write return "<< ret;
}

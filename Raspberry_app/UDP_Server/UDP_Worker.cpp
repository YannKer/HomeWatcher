#include "UDP_Worker.h"

#include <QDebug>
#include <QTextCodec>
#include <QThread>




UDP_Worker::UDP_Worker(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Lancement du serveur UDP..." ;

   m_UdpSocket = new QUdpSocket(this);
   m_UdpSocket->bind(QHostAddress("192.168.0.31"), 7755);

   connect(m_UdpSocket, &QUdpSocket::readyRead,
           this, &UDP_Worker::readPendingDatagrams);





}

void UDP_Worker::readPendingDatagrams()
{
   qDebug() << __PRETTY_FUNCTION__;
   while (m_UdpSocket->hasPendingDatagrams()) {
       QNetworkDatagram datagram = m_UdpSocket->receiveDatagram();
       qDebug() <<"read "<< datagram.data()<<" from "<<datagram.senderAddress()<<":"<<datagram.senderPort();

       processTheDatagram(datagram);
   }
}


void UDP_Worker::process()
{
    if(!m_Camera->isOpened())
    {
        //Open camera
        qDebug() <<"Opening Camera";
        if ( !m_Camera->open()) {qDebug()<<"Error opening camera"; return;}
        //wait a while until camera stabilizes
        qDebug()<<"Sleeping for 3 secs";
        QThread::msleep(3000);
        m_data=new unsigned char[  m_Camera->getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
        m_imgCpt = 0;

    }

    //capture
    m_Camera->grab();

    //extract the image in rgb format
    m_Camera->retrieve ( m_data );//get camera image

    uint16_t width = m_Camera->getWidth();
    uint16_t height = m_Camera->getHeight();


    QByteArray byteArrayImage;
    QDataStream streamImage(&byteArrayImage, QIODevice::WriteOnly);
    streamImage <<(uint8_t)0xFF;
    streamImage <<(uint8_t)0x01;
    streamImage << width;
    streamImage << height;

    m_UdpSocket->write(byteArrayImage);

    int imgsize = width*height*3;
    int nbpaket = imgsize/3000;
    for(int i=0;i<nbpaket;++i)
    {
        byteArrayImage.clear();
        streamImage << i;
        streamImage.writeBytes((char*)(m_data+3000*i),3000);
        m_UdpSocket->write(byteArrayImage);
    }

    qDebug() << "write image "<<m_imgCpt++;
}

void UDP_Worker::processTheDatagram(QNetworkDatagram datagram)
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << datagram.data();
    if( datagram.data() == QByteArray("INIT_CONNECTION"))
    {
        qDebug() << "INIT_CONNECTION "<<datagram.senderAddress()<<":"<<datagram.senderPort();
        m_UdpSocket->connectToHost(datagram.senderAddress(), datagram.senderPort(),QIODevice::ReadWrite);
        emit connected();
    }
    else if(datagram.data() == QByteArray("CLOSE_CONNECTION"))
    {

    }


}

void UDP_Worker::sendMessage(QString msg)
{
    //qDebug() << __PRETTY_FUNCTION__;
    auto ret = m_UdpSocket->write(msg.toUtf8(),msg.length());
    qDebug() << "write return "<< ret;
}

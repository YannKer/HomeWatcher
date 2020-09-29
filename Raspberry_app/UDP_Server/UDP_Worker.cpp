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


   m_Camera = new raspicam::RaspiCam();


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

    }

    uint imgSize = m_Camera->getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB );
    //capture
    m_Camera->grab();

    //extract the image in rgb format
    m_Camera->retrieve ( m_data );//get camera image
    auto format = m_Camera->getFormat();
    uint width = m_Camera->getWidth();
    uint height = m_Camera->getHeight();

    m_image = QImage(m_data,width,height,QImage::Format_RGB888);

    bool convertFromImage = m_pixmap.convertFromImage(m_image);

    emit newImage(m_pixmap);
}

void UDP_Worker::processTheDatagram(QNetworkDatagram datagram)
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << datagram.data();
    if( datagram.data() == QByteArray("INIT_CONNECTION"))
    {
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

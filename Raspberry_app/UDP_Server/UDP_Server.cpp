#include "UDP_Server.h"
#include "Camera/Camera.h"


std::unique_ptr< UDP_Server > UDP_Server::m_this;


//--------------------------------------------------------------------------
UDP_Server &UDP_Server::instance()
{
    if( !m_this ) {
        assert( false );
    }
    return *m_this;
}

//--------------------------------------------------------------------------
void UDP_Server::init()
{
    if( m_this ) {
        qDebug("service AblSettings already initialized.");
        assert( false );
        return;
    }
    m_this = std::unique_ptr< UDP_Server >(new UDP_Server());
}

//--------------------------------------------------------------------------
void UDP_Server::shutdown()
{
    if( !m_this ) {
        qDebug("service AblSettings not initialized.");
        assert( false );
        return;
    }

    m_this.reset();
}

//--------------------------------------------------------------------------
UDP_Server::UDP_Server()
    : QObject()
{
    m_Camera = new Camera(this);
    m_UdpSocket = new QUdpSocket(this);
    m_UdpSocket->bind(QHostAddress("192.168.0.31"), 7755);
    //m_UdpSocket->connectToHost(QHostAddress("192.168.0.23"), 7755,QIODevice::ReadWrite);

    connect(m_UdpSocket, &QUdpSocket::readyRead, this, &UDP_Server::readPendingDatagrams);
    connect(m_Camera, &Camera::newImage, this, &UDP_Server::onCameraNewImage);

    m_timer = new QTimer();

//    QObject::connect(m_timer, SIGNAL (timeout()), m_worker, SLOT (process()));
//    QObject::connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
//    QObject::connect(m_worker, SIGNAL (connected()), m_timer, SLOT (start()));
//    QObject::connect(m_worker, SIGNAL (disconnected()), m_timer, SLOT (stop()));
//    QObject::connect(m_worker, SIGNAL (newImage(QPixmap)), this, SIGNAL (newImage(QPixmap)));
//    thread->start();
//    m_timer->setInterval(50);
}

void UDP_Server::sendMessage(QString msg)
{
    //qDebug() << __PRETTY_FUNCTION__;
    //m_worker->sendMessage(msg);
}

void UDP_Server::readPendingDatagrams()
{
    //qDebug() << __PRETTY_FUNCTION__;
    while (m_UdpSocket->hasPendingDatagrams()) {
           QNetworkDatagram datagram = m_UdpSocket->receiveDatagram();
           qDebug() <<"read "<< datagram.data()<<" from "<<datagram.senderAddress()<<":"<<datagram.senderPort();
           processTheDatagram(datagram);
       }
}

void UDP_Server::processTheDatagram(QNetworkDatagram datagram)
{
    qDebug() << __PRETTY_FUNCTION__;
//    qDebug() << datagram.data();

    QByteArray byteArrayImage = datagram.data();
    QDataStream streamImage(&byteArrayImage, QIODevice::ReadOnly);

    if(byteArrayImage.size() >= 2)
    {
        uint8_t CMD_first;
        uint8_t CMD_seconde;

        streamImage >> CMD_first;
        streamImage >> CMD_seconde;

        if( CMD_first == 0xFF && CMD_seconde == 0x01)
        {
            m_FrameID = 0;
            m_Camera->start();
        }
        else if( CMD_first == 0xFF && CMD_seconde == 0x02)
        {
            m_Camera->stop();
        }
    }
}

void UDP_Server::onCameraNewImage( )
{



    Image img = m_Camera->popImage();



    int imgsize = img.width()*img.height()*3;
    int nbpaket = imgsize/1200;
    //char*packet = new char[1200];
    uint32_t packetoffset =0;
    qDebug()<<"nbpaket = "<<nbpaket;
    for(int packetID=0;packetID<nbpaket-1;++packetID)
    {
        QByteArray byteArrayImage;
        QDataStream streamImage(&byteArrayImage, QIODevice::WriteOnly);
        packetoffset = (uint32_t)(1200*packetID);
        streamImage << (uint8_t)0xFF;
        streamImage << (uint8_t)0x03;
        streamImage << (uint16_t)img.width();
        streamImage << (uint16_t)img.height();
        streamImage << (uint32_t)m_FrameID;
        streamImage << packetoffset;
        byteArrayImage.append((char*)&img.data()[packetoffset],1200);
        m_UdpSocket->writeDatagram(byteArrayImage,QHostAddress("192.168.0.23"), 7755);


//        qDebug()<<"m_width = "<<img.width();
//        qDebug()<<"m_height = "<<img.height();
//        qDebug()<<"frameID = "<<m_FrameID;
        qDebug()<<"packetOffset = "<<(uint32_t)(1200*packetID);
        qDebug()<<"packetID = "<<packetoffset/1200;

        //qDebug()<<"16 first bytes = "<<byteArrayImage.left(30);
    }

    ++m_FrameID;
}

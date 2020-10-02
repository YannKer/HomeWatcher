#include "UDP_Server.h"
#include "UDP_Worker.h"

#include <QThread>


std::unique_ptr< UDP_Server > UDP_Server::m_this;

//--------------------------------------------------------------------------
UDP_Server::UDP_Server()
    : QObject()
{
    m_UdpSocket = new QUdpSocket(this);
    m_UdpSocket->bind(QHostAddress("192.168.0.23"), 7755);
    //m_UdpSocket->connectToHost(QHostAddress("192.168.0.31"), 7755);

    connect(m_UdpSocket, &QUdpSocket::readyRead,this, &UDP_Server::readPendingDatagrams);

    m_dataPacket = new char[1200];
}

//--------------------------------------------------------------------------
void UDP_Server::sendMessage(QString msg)
{
    QByteArray byteArrayImage;
    QDataStream streamImage(&byteArrayImage, QIODevice::WriteOnly);
    bool to_send = false;

    if(msg == "START_CAMERA")
    {
        m_currentFrameID = -1;
        streamImage <<(uint8_t)0xFF;
        streamImage <<(uint8_t)0x01;
        to_send = true;
    }
    else if (msg == "STOP_CAMERA")
    {
        streamImage <<(uint8_t)0xFF;
        streamImage <<(uint8_t)0x02;
        to_send = true;
    }

    if(to_send)
    {
        //m_UdpSocket->write(byteArrayImage);
        m_UdpSocket->writeDatagram(byteArrayImage,QHostAddress("192.168.0.31"), 7755);
    }
}

//--------------------------------------------------------------------------
void UDP_Server::readPendingDatagrams()
{

    while (m_UdpSocket->hasPendingDatagrams()) {
           QNetworkDatagram datagram = m_UdpSocket->receiveDatagram();
           //qDebug() <<"read "<< datagram.data()<<" from "<<datagram.senderAddress()<<":"<<datagram.senderPort();
           processTheDatagram(datagram);
       }
}
//--------------------------------------------------------------------------
void UDP_Server::processTheDatagram(QNetworkDatagram datagram)
{
    qDebug() << __PRETTY_FUNCTION__;
//    qDebug() << datagram.data();

    QByteArray byteArrayImage = datagram.data();

    if (byteArrayImage.size() >= 2)
    {

        QDataStream streamImage(&byteArrayImage, QIODevice::ReadOnly);


            qDebug()<<"byteArrayImage.size() = "<<byteArrayImage.size();
            //qDebug()<<"16 first bytes = "<<byteArrayImage.left(30);
            uint8_t CMD_first;
            uint8_t CMD_seconde;

            streamImage >> CMD_first;
            streamImage >> CMD_seconde;



            if( CMD_first == 0xFF && CMD_seconde == 0x03)
            {
                uint32_t frameID;
                uint32_t packetOffset;

                streamImage >> m_width;
                streamImage >> m_height;
                streamImage >> frameID;
                streamImage >> packetOffset;

    //            qDebug()<<"m_width = "<<m_width;
    //            qDebug()<<"m_height = "<<m_height;
                //qDebug()<<"frameID = "<<frameID;
                qDebug()<<"packetOffset = "<<packetOffset;
                qDebug()<<"packetID = "<<packetOffset/1200;

                if(m_currentFrameID == -1)
                {
                    m_currentFrameID = frameID;
                }
                else if(m_currentFrameID<frameID)
                {
                    sendCurrentFrame();
                    m_currentFrameID = frameID;
                }


                if(m_dataImageSize != m_width*m_height*3)
                {
                    if(m_dataImage!=nullptr)
                        free(m_dataImage);
                    m_dataImageSize = m_width*m_height*3;
                    m_dataImage = new char[m_dataImageSize];
                }


                uint32_t packetSize = 1200;
                //streamImage.readBytes(m_dataPacket,packetSize);


                //qDebug()<<"m_dataPacket = "<<(int)m_dataPacket[0]<<","<<(int)m_dataPacket[1]<<","<<(int)m_dataPacket[2]<<","<<(int)m_dataPacket[3];
                memcpy(&m_dataImage[packetOffset],byteArrayImage.data()+14,1200);

        }
//        else
//        {
//            break;
//        }
    }
}

//--------------------------------------------------------------------------
void UDP_Server::sendCurrentFrame()
{
    qDebug(__PRETTY_FUNCTION__);
    m_image = QImage((uchar*)m_dataImage,m_width,m_height,QImage::Format_RGB888);
    bool convertFromImage = m_pixmap.convertFromImage(m_image);
    if(convertFromImage)
        emit newImage(m_pixmap);

    std::fill(m_dataImage,m_dataImage+m_dataImageSize,0);
}

















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

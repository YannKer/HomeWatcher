#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <QTimer>
#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <memory>

class UDP_Worker;

class UDP_Server : public QObject
{
    Q_OBJECT
public:
    static void init();
    static void shutdown();

    static UDP_Server & instance();

    void sendMessage(QString msg);

signals:
    void newImage(QPixmap);

public slots:

private:
    QUdpSocket * m_UdpSocket;


    char * m_dataPacket = nullptr;
    char * m_dataImage = nullptr;
    size_t m_dataImageSize = 0;
    QPixmap m_pixmap;
    QImage m_image;
    uint16_t m_width = 0;
    uint16_t m_height = 0;
    int m_currentFrameID;

    //--------------------------------------------------------------------------
    static std::unique_ptr< UDP_Server > m_this;
    explicit UDP_Server();
    void sendCurrentFrame();

private slots:
    void readPendingDatagrams();
    void processTheDatagram(QNetworkDatagram datagram);
};

#endif // UDP_SERVER_H

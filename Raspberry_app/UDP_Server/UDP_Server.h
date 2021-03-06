#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <QTimer>
#include <QObject>
#include <QPixmap>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <memory>

class Camera;

class UDP_Server : public QObject
{
    Q_OBJECT
public:
    static void init();
    static void shutdown();

    static UDP_Server & instance();

    void sendMessage(QString msg);

signals:
    void connected();
    void disconnected();
    void newImage(QPixmap);

private:

    QTimer* m_timer;

    QUdpSocket * m_UdpSocket;
    Camera * m_Camera;
    int m_FrameID;

    //--------------------------------------------------------------------------
    static std::unique_ptr< UDP_Server > m_this;
    explicit UDP_Server();

private slots:
    void readPendingDatagrams();
    void processTheDatagram(QNetworkDatagram datagram);
    void onCameraNewImage();
};

#endif // UDP_SERVER_H

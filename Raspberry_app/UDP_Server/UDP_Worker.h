#ifndef UDP_WORKER_H
#define UDP_WORKER_H

#include <QObject>
#include <QPixmap>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <ctime>
#include <iostream>
#include "raspicam/raspicam.h"


class UDP_Worker : public QObject
{
    Q_OBJECT
public:
    explicit UDP_Worker(QObject *parent = nullptr);

    void sendMessage(QString msg);

signals:
    void connected();
    void disconnected();
    void newImage(QPixmap img);

public slots:
    void process();

private:

    QUdpSocket * m_UdpSocket;
    raspicam::RaspiCam * m_Camera;
    unsigned char *m_data;
    QPixmap m_pixmap;
    QImage m_image;
private slots:
    void readPendingDatagrams();
    void processTheDatagram(QNetworkDatagram datagram);


};

#endif // UDP_WORKER_H

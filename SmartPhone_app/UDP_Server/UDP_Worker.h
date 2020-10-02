#ifndef UDP_WORKER_H
#define UDP_WORKER_H

#include <QObject>
#include <QPixmap>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QImage>

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
    void newImage(QPixmap);

public slots:
    void process();

private:

    QUdpSocket * m_UdpSocket;
    QVector<char> m_dataVector;
    char * m_data = nullptr;
    QPixmap m_pixmap;
    QImage m_image;
    uint16_t width = 0;
    uint16_t height = 0;

private slots:
    void readPendingDatagrams();
    void processTheDatagram(QNetworkDatagram datagram);


};

#endif // UDP_WORKER_H

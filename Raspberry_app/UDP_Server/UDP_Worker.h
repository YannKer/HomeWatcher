#ifndef UDP_WORKER_H
#define UDP_WORKER_H

#include <QObject>
#include <QPixmap>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <ctime>
#include <iostream>



class UDP_Worker : public QObject
{
    Q_OBJECT
public:
    explicit UDP_Worker(QObject *parent = nullptr);

    void sendMessage(QString msg);



};

#endif // UDP_WORKER_H

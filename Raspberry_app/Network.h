#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QUdpSocket>

class Network : public QObject
{
    Q_OBJECT

public:
    Network();
    ~Network();

    void initSocket();
    void readPendingDatagrams();
    QUdpSocket * m_udpSocket;
};

#endif // NETWORK_H

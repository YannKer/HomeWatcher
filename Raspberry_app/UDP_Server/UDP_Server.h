#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <QTimer>
#include <QObject>
#include <QPixmap>
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

    QTimer* m_timer;
    UDP_Worker* m_worker;

    //--------------------------------------------------------------------------
    static std::unique_ptr< UDP_Server > m_this;
    explicit UDP_Server();
};

#endif // UDP_SERVER_H

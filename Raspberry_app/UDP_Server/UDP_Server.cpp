#include "UDP_Server.h"
#include "UDP_Worker.h"

#include <QThread>


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
    m_timer = new QTimer();
    QThread* thread = new QThread;
    m_worker = new UDP_Worker();
    m_worker->moveToThread(thread);

    QObject::connect(m_timer, SIGNAL (timeout()), m_worker, SLOT (process()));
    QObject::connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
    QObject::connect(m_worker, SIGNAL (connected()), m_timer, SLOT (start()));
    QObject::connect(m_worker, SIGNAL (disconnected()), m_timer, SLOT (stop()));
    QObject::connect(m_worker, SIGNAL (newImage(QPixmap)), this, SIGNAL (newImage(QPixmap)));
    thread->start();
    m_timer->setInterval(500);
}

void UDP_Server::sendMessage(QString msg)
{
    //qDebug() << __PRETTY_FUNCTION__;
    m_worker->sendMessage(msg);
}

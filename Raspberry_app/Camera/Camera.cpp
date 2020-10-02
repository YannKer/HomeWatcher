#include "Camera.h"
#include<QtConcurrent>

Camera::Camera(QObject *parent) : QObject(parent)
{
    m_Camera = new raspicam::RaspiCam();
    m_Camera->setFormat(raspicam::RASPICAM_FORMAT_RGB);
    m_Camera->setFrameRate(2);
    m_Camera->open(false);

    m_imgSizeBytes = m_Camera->getImageBufferSize();
    m_image = QImage(m_Camera->getWidth(),m_Camera->getHeight(),QImage::Format_RGB888);
    m_queue = new SharedQueue<Image>(10);
}

void Camera::start(){
    m_Camera->startCapture();
    m_continue = true;
    m_future = QtConcurrent::run(this, &Camera::grabFrames );

}

void Camera::stop()
{
    m_continue = false;
    m_future.waitForFinished();

    m_Camera->release();
    if(!m_Camera->isOpened())
        m_Camera->open(false);

    if(m_queue->size())
    {
        m_queue->pop_front();
    }
}

void Camera::grabFrames()
{
    int cptImg = 0;
    while (m_continue) {
        Image img(m_Camera->getWidth(),m_Camera->getHeight());

        m_Camera->grab();
        m_Camera->retrieve ( img.data().data() );//get camera image



        /*        Image img(160,120);

        m_Camera->grab();
        m_Camera->retrieve ( tmp );//get camera image
        m_image = QImage(1280,960,QImage::Format_RGB888);
        m_image.fromData(tmp,1280*960*3);
        m_image.scaled(160,120);
        m_image.bits();
        memmove(img.data().data(), m_image.bits(), 160*120*3);*/

        m_queue->push_back(img);

        emit newImage();
        qDebug()<<"Frame "<<cptImg++<<" grabbed";
    }
}

Image Camera::popImage()
{
    Image toret = m_queue->front();
    m_queue->pop_front();
    return toret;
}

#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <QObject>
#include <QFuture>
#include<QImage>

#include "raspicam/raspicam.h"
#include "SharedQueue.h"

class Image
{
public:
    Image(uint16_t width,uint16_t height) : m_width(width),m_height(height){m_data.resize(width*height*3);}
    ~Image() {m_data.clear();}

    uint16_t width(){return m_width;}
    uint16_t height(){return m_height;}
    std::vector<uchar> & data(){return m_data;}
private:
    uint16_t m_width=0;
    uint16_t m_height=0;
    std::vector<uchar> m_data;
}  ;

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

    void start();
    void stop();

    Image popImage();

signals:
    void newImage();

private:
    raspicam::RaspiCam * m_Camera;
    bool m_continue = false;
    QFuture<void> m_future;
    QImage m_image;
    size_t m_imgSizeBytes;
    uchar tmp[1280*960*3];

    SharedQueue<Image> * m_queue;

private slots:
    void grabFrames();
};

#endif // CAMERA_H

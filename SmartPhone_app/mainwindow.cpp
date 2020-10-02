#include "mainwindow.h"
#include "UDP_Server/UDP_Server.h"

#include<QFormLayout>
#include<QLineEdit>
#include<QPushButton>
#include<QCameraInfo>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(500,500);
    setStyleSheet("QMainWindow {background: \
                  qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,\
                  stop: 0 #A0A0FF, stop: 1.0 #FBFBFF);}");


    QWidget * centralWidget = new QWidget(this);
    QFormLayout * flayout = new QFormLayout(centralWidget);
    QLineEdit * LE_msg = new QLineEdit("INIT_CONNECTION",centralWidget);
    QPushButton * PB_sendMsh = new QPushButton("OK",centralWidget);
    QPushButton * PB_startCamera = new QPushButton("Start Camera",centralWidget);
    QPushButton * PB_stopCamera = new QPushButton("Stop Camera",centralWidget);
    QHBoxLayout * HBlayout_sendMsg = new QHBoxLayout();
    HBlayout_sendMsg->addWidget(LE_msg);
    HBlayout_sendMsg->addWidget(PB_sendMsh);
    flayout->addRow("Send message : ",HBlayout_sendMsg);
    flayout->addRow("",PB_startCamera);
    flayout->addRow("",PB_stopCamera);
    flayout->addRow("Image",&m_labelImage);

    connect(&UDP_Server::instance(),&UDP_Server::newImage, this, &MainWindow::displayImg);
    connect(PB_startCamera,&QPushButton::clicked,this, [=](){UDP_Server::instance().sendMessage("START_CAMERA");});
    connect(PB_stopCamera,&QPushButton::clicked,this, [=](){UDP_Server::instance().sendMessage("STOP_CAMERA");});
    connect(PB_sendMsh,&QPushButton::clicked,this, [=](){UDP_Server::instance().sendMessage(LE_msg->text());});



    m_labelImage.setFixedSize(1000,1000);
    m_labelImage.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setCentralWidget(centralWidget);


    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::displayImg(QPixmap img)
{
    qDebug()<<__PRETTY_FUNCTION__;
    m_labelImage.setPixmap(img.scaled(m_labelImage.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

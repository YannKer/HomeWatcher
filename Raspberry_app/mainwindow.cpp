#include "mainwindow.h"
#include "UDP_Server/UDP_Server.h"

#include<QDebug>
#include<QFormLayout>
#include<QLineEdit>
#include<QPushButton>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(500,500);
    setStyleSheet("QMainWindow {background: \
                  qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,\
                  stop: 0 #A0A0FF, stop: 1.0 #FBFBFF);}");

    QWidget * centralWidget = new QWidget(this);
    QFormLayout * flayout = new QFormLayout(centralWidget);
    QLineEdit * LE_msg = new QLineEdit(centralWidget);
    QPushButton * PB_sendMsh = new QPushButton("OK",centralWidget);
    QHBoxLayout * HBlayout_sendMsg = new QHBoxLayout();
    HBlayout_sendMsg->addWidget(LE_msg);
    HBlayout_sendMsg->addWidget(PB_sendMsh);
    flayout->addRow("Send message : ",HBlayout_sendMsg);

    connect(PB_sendMsh,&QPushButton::clicked,this, [=](){UDP_Server::instance().sendMessage(LE_msg->text());});



}

MainWindow::~MainWindow()
{

}



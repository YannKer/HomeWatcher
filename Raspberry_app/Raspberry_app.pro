#-------------------------------------------------
#
# Project created by QtCreator 2019-03-07T16:21:44
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


INSTALLS       += target
TARGET = Raspberry_app
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 console

SOURCES += \
        UDP_Server/UDP_Server.cpp \
        UDP_Server/UDP_Worker.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        UDP_Server/UDP_Server.h \
        UDP_Server/UDP_Worker.h \
        mainwindow.h

CONFIG += mobility
MOBILITY = 

#INCLUDEPATH += /home/yannk/Bureau/raspi/sysroot/usr/local/include
#LIBS += -L"/home/yannk/Bureau/raspi/opencv4/lib" -lopencv_core
#LIBS += -L"/usr/local/lib" -lraspicam
INCLUDEPATH += "/usr/local/include"
#LIBS += -L"/home/yannk/Bureau/raspi/sysroot/usr/lib/arm-linux-gnueabihf/" -lopencv_core



# Default rules for deployment.
target.path     = /home/pi/Desktop/HW
!isEmpty(target.path): INSTALLS += target

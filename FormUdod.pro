#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T09:53:38
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FormUdod
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    info.cpp

HEADERS  += mainwindow.h \
    info.h

FORMS    += mainwindow.ui \
    info.ui

RESOURCES += \
    sourses.qrc

CONFIG += c++11

RC_ICONS = Icons\udod.ico

#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T05:25:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlsaRecorderGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    preferences.cpp

HEADERS  += mainwindow.h \
    preferences.h

FORMS    += mainwindow.ui \
    preferences.ui

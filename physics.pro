#-------------------------------------------------
#
# Project created by QtCreator 2011-01-08T00:07:45
#
#-------------------------------------------------

QT       += core gui

TARGET = physics
TEMPLATE = app

INCLUDEPATH += "/usr/include/qwt-qt4/"
LIBS += -lqwt-qt4

SOURCES += main.cpp\
        mainwindow.cpp \
    generatorthread.cpp \
    storagethread.cpp \
    processorthread.cpp

HEADERS  += mainwindow.h \
    generatorthread.h \
    storagethread.h \
    processorthread.h

FORMS    += mainwindow.ui

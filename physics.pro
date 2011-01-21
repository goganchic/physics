#-------------------------------------------------
#
# Project created by QtCreator 2011-01-08T00:07:45
#
#-------------------------------------------------

QT       += core gui

TARGET = physics
TEMPLATE = app

unix {
    INCLUDEPATH += "/usr/include/qwt-qt4/"
    LIBS += -lqwt-qt4
}

win32 {
    QWT_LOCATION = C:/qwt-5.2.1 # Qwt location (must match exactly to where you unzip the files)
    INCLUDEPATH += $${QWT_LOCATION}/src
    LIBS = -L$${QWT_LOCATION}/lib \
        -lqwt

    # The following section is to select static lib base on debug or release build
    if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
        LIBS = $$member(LIBS, 0) \
            $$member(LIBS, 1)d
    }
}


SOURCES += main.cpp\
        mainwindow.cpp \
    generatorthread.cpp \
    storagethread.cpp \
    processorthread.cpp \
    resultswindow.cpp \
    resultstablemodel.cpp

HEADERS  += mainwindow.h \
    generatorthread.h \
    storagethread.h \
    processorthread.h \
    resultswindow.h \
    resultstablemodel.h

FORMS    += mainwindow.ui \
    resultswindow.ui

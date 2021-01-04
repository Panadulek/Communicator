QT       += core gui widgets network
CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    connectiondialog.cpp \
    localcontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    protocol.cpp \
    senderwidget.cpp \
    serverinfoitem.cpp \
    useritem.cpp

HEADERS += \
    connectiondialog.h \
    localcontroller.h \
    mainwindow.h \
    protocol.h \
    ../protocolheaders.h \
    senderwidget.h \
    serverinfoitem.h \
    useritem.h


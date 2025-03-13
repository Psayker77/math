QT += testlib sql network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_serverfunction.cpp \
    ../server/database.cpp \
    ../server/mytcpserver.cpp \
    ../server/tasks.cpp \
    ../server/serverfunction.cpp

SUBDIRS += \
    ../server/echoServer.pro

HEADERS += \
    ../server/database.h \
    ../server/mytcpserver.h \
    ../server/tasks.h \
    ../server/serverfunction.h

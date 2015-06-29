#-------------------------------------------------
#
# Project created by QtCreator 2014-02-09T11:08:27
#
#-------------------------------------------------

QT       += core gui network
unix:QMAKE_CXXFLAGS += -std=c++11
win32-g++:QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FunctionalClientTest
TEMPLATE = app


SOURCES += main.cpp\
	maindialog.cpp \
    qghtcpclient.cpp

HEADERS  += maindialog.h \
    qghtcpclient.h

FORMS    += maindialog.ui

OTHER_FILES += \
	FunctionalClientTest_zh_CN.ts
TRANSLATIONS += FunctionalClientTest_zh_CN.ts

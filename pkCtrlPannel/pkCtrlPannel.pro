#-------------------------------------------------
#
# Project created by QtCreator 2014-12-02T14:25:49
#
#-------------------------------------------------

QT       += core gui sql network
unix:QMAKE_CXXFLAGS += -std=c++11
win32-g++:QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pkCtrlPannel
TEMPLATE = app


SOURCES += main.cpp\
	mainwndctrlpannel.cpp \
    dialoglogin.cpp \
    dialogsettings.cpp \
    qcoloriconsqlmodel.cpp \
    pklts_ctrl/pklts_methods.cpp

HEADERS  += mainwndctrlpannel.h \
    dialoglogin.h \
    dialogsettings.h \
    qcoloriconsqlmodel.h \
    pklts_ctrl/st_ctrlmsg.h \
    pklts_ctrl/pklts_methods.h

FORMS    += mainwndctrlpannel.ui \
    dialoglogin.ui \
    dialogsettings.ui

RESOURCES += \
    resources.qrc
OTHER_FILES += \
	pkCtrlPannel_zh_CN.ts
TRANSLATIONS +=pkCtrlPannel_zh_CN.ts

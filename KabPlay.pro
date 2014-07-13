#-------------------------------------------------
#
# Project created by QtCreator 2014-07-11T14:36:54
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KabPlay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    infomainwindow.cpp \
    kqui_collapsibleframe.cpp \
    kqtablewidget.cpp \
    kqrowdata.cpp \
    timermainwindow.cpp \
    mainwindowbase.cpp \
    kqtitleframe.cpp \
    submainwindow.cpp \
    kqtime.cpp \
    customtabledelegate.cpp \
    kandata.cpp

HEADERS  += mainwindow.h \
    infomainwindow.h \
    kqui_collapsibleframe.h \
    kqtablewidget.h \
    kqrowdata.h \
    timermainwindow.h \
    mainwindowbase.h \
    kqtitleframe.h \
    submainwindow.h \
    kqtime.h \
    customtabledelegate.h \
    kandata.h

FORMS    += mainwindow.ui \
    infomainwindow.ui \
    kqui_collapsibleframe.ui \
    timermainwindow.ui

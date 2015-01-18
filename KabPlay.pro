#-------------------------------------------------
#
# Project created by QtCreator 2014-07-11T14:36:54
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets axcontainer webkitwidgets winextras

TARGET = KabPlay
TEMPLATE = app

TYPELIBS = $$system(dumpcpp -getfile {8A497A30-FE7D-445F-8082-8ED319E1B386})

SOURCES += main.cpp\
        mainwindow.cpp \
    infomainwindow.cpp \
    kqui_collapsibleframe.cpp \
    kqtablewidget.cpp \
    timermainwindow.cpp \
    mainwindowbase.cpp \
    kqtitleframe.cpp \
    submainwindow.cpp \
    kqtime.cpp \
    customtabledelegate.cpp \
    kandata.cpp \
    kansavedata.cpp \
    kanreqdata.cpp \
    kqrowdata.cpp \
    cookiejar.cpp \
    qnetworkproxyfactoryset.cpp \
    kandataconnector.cpp \
    kandatacalc.cpp \
    kqwebpage.cpp \
    klog.cpp \
    kqnetworkaccessmanager.cpp \
    weaponmainwindow.cpp

HEADERS  += mainwindow.h \
    infomainwindow.h \
    kqui_collapsibleframe.h \
    kqtablewidget.h \
    timermainwindow.h \
    mainwindowbase.h \
    kqtitleframe.h \
    submainwindow.h \
    kqtime.h \
    customtabledelegate.h \
    kandata.h \
    kansavedata.h \
    kanreqdata.h \
    kqrowdata.h \
    cookiejar.h \
    qnetworkproxyfactoryset.h \
    kandataconnector.h \
    kandatacalc.h \
    kqwebpage.h \
    klog.h \
    kqnetworkaccessmanager.h \
    weaponmainwindow.h

FORMS    += mainwindow.ui \
    infomainwindow.ui \
    kqui_collapsibleframe.ui \
    timermainwindow.ui \
    weaponmainwindow.ui

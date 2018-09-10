#-------------------------------------------------
#
# Project created by QtCreator 2014-07-11T14:36:54
#
#-------------------------------------------------

QT       += core gui multimedia network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    greaterThan(QT_MINOR_VERSION, 5) {
        QT += webenginewidgets
    } else {
        QT += webkitwidgets
    }
}

windows: QT += axcontainer winextras

msvc: LIBS += -luser32 -lole32
LIBS += -lm

DEFINES += NOMINMAX

TARGET = KanPlay
TEMPLATE = app

#TYPELIBS = $$system(dumpcpp -getfile {8A497A30-FE7D-445F-8082-8ED319E1B386})

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
    klog.cpp \
    kqnetworkaccessmanager.cpp \
    kqnetworkreply.cpp \
    weaponmainwindow.cpp \
    ControlAction.cpp \
    ControlManager.cpp \
    ExpeditionManager.cpp \
    kandataconnector_parse.cpp \
    kandataconnector_register.cpp \
    kandataconnector_update.cpp \
    kqmaintitleframe.cpp \
    kqwebpage.cpp \
    mainwindowevents.cpp \
    mainwindowjob.cpp \
    mainwindowsettings.cpp \
    qwebenginemouseeventfilter.cpp \
    qwindowseventfilter.cpp \
    shipmainwindow.cpp \
    anyactionselectdialog.cpp \
    developactionselectdialog.cpp \
    RemoteNotifyHandler.cpp \
    ActionCheckAndClickDefine.cpp \
    KQFrame.cpp

windows: SOURCES += \
    shdocvw.cpp \
    nekoxy.cpp \
    fidcom.cpp \
    titanium_web_proxy.cpp

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
    klog.h \
    kqnetworkaccessmanager.h \
    kqnetworkreply.h \
    weaponmainwindow.h \
    ControlAction.h \
    ControlManager.h \
    ExpeditionManager.h \
    kqmaintitleframe.h \
    kqwebpage.h \
    qwebenginemouseeventfilter.h \
    qwindowseventfilter.h \
    shipmainwindow.h \
    anyactionselectdialog.h \
    developactionselectdialog.h \
    RemoteNotifyHandler.h \
    ActionCheckAndClickDefine.h \
    KQFrame.h

windowes: HEADERS += \
    shdocvw.h \
    nekoxy.h \
    fidcom.h \
    titanium_web_proxy.h

FORMS    += mainwindow.ui \
    infomainwindow.ui \
    kqui_collapsibleframe.ui \
    timermainwindow.ui \
    weaponmainwindow.ui \
    shipmainwindow.ui \
    anyactionselectdialog.ui \
    developactionselectdialog.ui

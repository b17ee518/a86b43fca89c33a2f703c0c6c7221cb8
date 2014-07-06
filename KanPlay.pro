#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T12:09:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KanPlay
TEMPLATE = app


SOURCES += main.cpp\
        kqmainwindow.cpp \
    kqwidgetinterface.cpp \
    kqmainwindowbase.cpp \
    kqsubmainwindowbase.cpp \
    kqframebase.cpp \
    kqtitleframe.cpp \
    kqcontentframebase.cpp \
    kqinfocontentframe.cpp \
    kqtimercontentframebase.cpp \
    kqtablewidgetbase.cpp \
    kqtablewidget.cpp \
    kqcollapsibletablewidget.cpp \
    kqinfosubmainwindow.cpp \
    kqtimersubmainwindow.cpp \
    kqmaintitleframe.cpp \
    kqpushbuttonbase.cpp \
    kqpushbutton.cpp \
    kqui_maintitleframe.cpp \
    kqcomboboxbase.cpp \
    kqcombobox.cpp \
    kqui_subtitleframe.cpp

HEADERS  += kqmainwindow.h \
    kqwidgetinterface.h \
    kqmainwindowbase.h \
    kqsubmainwindowbase.h \
    kqframebase.h \
    kqtitleframe.h \
    kqcontentframebase.h \
    kqinfocontentframe.h \
    kqtimercontentframebase.h \
    kqtablewidgetbase.h \
    kqtablewidget.h \
    kqcollapsibletablewidget.h \
    kqinfosubmainwindow.h \
    kqtimersubmainwindow.h \
    kqmaintitleframe.h \
    kqpushbutton.h \
    kqpushbuttonbase.h \
    kqui_maintitleframe.h \
    kqcomboboxbase.h \
    kqcombobox.h \
    kqui_subtitleframe.h

FORMS    += kqmainwindow.ui \
    kqinfosubmainwindow.ui \
    kqtimersubmainwindow.ui \
    kqui_maintitleframe.ui \
    kqui_subtitleframe.ui

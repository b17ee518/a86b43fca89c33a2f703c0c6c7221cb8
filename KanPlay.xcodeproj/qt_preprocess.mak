#############################################################################
# Makefile for building: KanPlay.app/Contents/MacOS/KanPlay
# Generated by qmake (3.0) (Qt 5.4.1)
# Project:  KanPlay.pro
# Template: app
# Command: /Users/cf4146/Qt5.4.1/5.4/clang_64/bin/qmake -spec macx-xcode -o KanPlay.xcodeproj/project.pbxproj KanPlay.pro
#############################################################################

MAKEFILE      = project.pbxproj

MOC       = /Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc
UIC       = /Users/cf4146/Qt5.4.1/5.4/clang_64/bin/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DNOMINMAX -DQT_NO_DEBUG -DQT_MULTIMEDIA_LIB -DQT_WEBKITWIDGETS_LIB -DQT_WEBENGINEWIDGETS_LIB -DQT_WIDGETS_LIB -DQT_WEBKIT_LIB -DQT_WEBENGINE_LIB -DQT_QUICK_LIB -DQT_GUI_LIB -DQT_QML_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB
INCPATH       = -I. -I../Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Versions/5/Headers -I../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/System/Library/Frameworks/AGL.framework/Headers -I. -I../Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compilers: moc_mainwindow.cpp moc_infomainwindow.cpp moc_kqui_collapsibleframe.cpp\
	 moc_kqtablewidget.cpp moc_timermainwindow.cpp moc_mainwindowbase.cpp\
	 moc_kqtitleframe.cpp moc_submainwindow.cpp moc_cookiejar.cpp\
	 moc_kqnetworkaccessmanager.cpp moc_kqnetworkreply.cpp moc_weaponmainwindow.cpp\
	 moc_ControlAction.cpp moc_kqmaintitleframe.cpp moc_kqwebpage.cpp\
	 moc_qwebenginemouseeventfilter.cpp moc_shipmainwindow.cpp ui_mainwindow.h ui_infomainwindow.h ui_kqui_collapsibleframe.h\
	 ui_timermainwindow.h ui_weaponmainwindow.h ui_shipmainwindow.h
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all: moc_mainwindow.cpp moc_infomainwindow.cpp moc_kqui_collapsibleframe.cpp moc_kqtablewidget.cpp moc_timermainwindow.cpp moc_mainwindowbase.cpp moc_kqtitleframe.cpp moc_submainwindow.cpp moc_cookiejar.cpp moc_kqnetworkaccessmanager.cpp moc_kqnetworkreply.cpp moc_weaponmainwindow.cpp moc_ControlAction.cpp moc_kqmaintitleframe.cpp moc_kqwebpage.cpp moc_qwebenginemouseeventfilter.cpp moc_shipmainwindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_infomainwindow.cpp moc_kqui_collapsibleframe.cpp moc_kqtablewidget.cpp moc_timermainwindow.cpp moc_mainwindowbase.cpp moc_kqtitleframe.cpp moc_submainwindow.cpp moc_cookiejar.cpp moc_kqnetworkaccessmanager.cpp moc_kqnetworkreply.cpp moc_weaponmainwindow.cpp moc_ControlAction.cpp moc_kqmaintitleframe.cpp moc_kqwebpage.cpp moc_qwebenginemouseeventfilter.cpp moc_shipmainwindow.cpp
moc_mainwindow.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QtGlobal \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		mainwindowbase.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		infomainwindow.h \
		submainwindow.h \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QShowEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QHideEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		kqui_collapsibleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		kqtablewidget.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QPushButton \
		timermainwindow.h \
		kqtime.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QString \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QProgressBar \
		../Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Versions/5/Headers/QMediaPlayer \
		../Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Versions/5/Headers/QMediaPlaylist \
		weaponmainwindow.h \
		shipmainwindow.h \
		qnetworkproxyfactoryset.h \
		../Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Versions/5/Headers/QNetworkProxyFactory \
		../Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Versions/5/Headers/QNetworkReply \
		../Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Versions/5/Headers/QWebEngineView \
		../Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Versions/5/Headers/QWebView \
		shdocvw.h \
		fidcom.h \
		nekoxy.h \
		titanium_web_proxy.h \
		qwindowseventfilter.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QAbstractNativeEventFilter \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QFile \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTextStream \
		qwebenginemouseeventfilter.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTime \
		mainwindow.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib mainwindow.h -o moc_mainwindow.cpp

moc_infomainwindow.cpp: submainwindow.h \
		mainwindowbase.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QShowEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QHideEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		kqui_collapsibleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		kqtablewidget.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QPushButton \
		infomainwindow.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib infomainwindow.h -o moc_infomainwindow.cpp

moc_kqui_collapsibleframe.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		kqtablewidget.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QPushButton \
		kqui_collapsibleframe.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib kqui_collapsibleframe.h -o moc_kqui_collapsibleframe.cpp

moc_kqtablewidget.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		kqtablewidget.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib kqtablewidget.h -o moc_kqtablewidget.cpp

moc_timermainwindow.cpp: submainwindow.h \
		mainwindowbase.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QShowEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QHideEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqtime.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QString \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QProgressBar \
		../Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Versions/5/Headers/QMediaPlayer \
		../Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Versions/5/Headers/QMediaPlaylist \
		timermainwindow.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib timermainwindow.h -o moc_timermainwindow.cpp

moc_mainwindowbase.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		mainwindowbase.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib mainwindowbase.h -o moc_mainwindowbase.cpp

moc_kqtitleframe.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QMouseEvent \
		kqtitleframe.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib kqtitleframe.h -o moc_kqtitleframe.cpp

moc_submainwindow.cpp: mainwindowbase.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QShowEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QHideEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		submainwindow.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib submainwindow.h -o moc_submainwindow.cpp

moc_cookiejar.cpp: cookiejar.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib cookiejar.h -o moc_cookiejar.cpp

moc_kqnetworkaccessmanager.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Versions/5/Headers/QNetworkAccessManager \
		../Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Versions/5/Headers/QNetworkReply \
		kqnetworkaccessmanager.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib kqnetworkaccessmanager.h -o moc_kqnetworkaccessmanager.cpp

moc_kqnetworkreply.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Versions/5/Headers/QNetworkReply \
		kqnetworkreply.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib kqnetworkreply.h -o moc_kqnetworkreply.cpp

moc_weaponmainwindow.cpp: submainwindow.h \
		mainwindowbase.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QShowEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QHideEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		kqui_collapsibleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		kqtablewidget.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QPushButton \
		weaponmainwindow.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib weaponmainwindow.h -o moc_weaponmainwindow.cpp

moc_ControlAction.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QElapsedTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		ControlAction.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib ControlAction.h -o moc_ControlAction.cpp

moc_kqmaintitleframe.cpp: kqtitleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QMouseEvent \
		mainwindowbase.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		kqmaintitleframe.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib kqmaintitleframe.h -o moc_kqmaintitleframe.cpp

moc_kqwebpage.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QtGlobal \
		../Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Versions/5/Headers/QWebEnginePage \
		../Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Versions/5/Headers/QWebPage \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QApplication \
		kqwebpage.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib kqwebpage.h -o moc_kqwebpage.cpp

moc_qwebenginemouseeventfilter.cpp: ../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTime \
		qwebenginemouseeventfilter.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib qwebenginemouseeventfilter.h -o moc_qwebenginemouseeventfilter.cpp

moc_shipmainwindow.cpp: submainwindow.h \
		mainwindowbase.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QMainWindow \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QCloseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QResizeEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QShowEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QHideEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		kqui_collapsibleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		kqtablewidget.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QPushButton \
		shipmainwindow.h
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/mkspecs/macx-clang -I/Users/cf4146/a86b43fca89c33a2f703c0c6c7221cb8 -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtMultimedia.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKitWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngineWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebKit.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtWebEngine.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQuick.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtQml.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtNetwork.framework/Headers -I/Users/cf4146/Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Headers -F/Users/cf4146/Qt5.4.1/5.4/clang_64/lib shipmainwindow.h -o moc_shipmainwindow.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h ui_infomainwindow.h ui_kqui_collapsibleframe.h ui_timermainwindow.h ui_weaponmainwindow.h ui_shipmainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h ui_infomainwindow.h ui_kqui_collapsibleframe.h ui_timermainwindow.h ui_weaponmainwindow.h ui_shipmainwindow.h
ui_mainwindow.h: mainwindow.ui \
		kqtitleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QMouseEvent
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/uic mainwindow.ui -o ui_mainwindow.h

ui_infomainwindow.h: infomainwindow.ui \
		kqtitleframe.h \
		kqui_collapsibleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QMouseEvent \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		kqtablewidget.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QPushButton
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/uic infomainwindow.ui -o ui_infomainwindow.h

ui_kqui_collapsibleframe.h: kqui_collapsibleframe.ui \
		kqtablewidget.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QTableWidget \
		kqrowdata.h \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QObject \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QList \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QColor
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/uic kqui_collapsibleframe.ui -o ui_kqui_collapsibleframe.h

ui_timermainwindow.h: timermainwindow.ui \
		kqtitleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QMouseEvent
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/uic timermainwindow.ui -o ui_timermainwindow.h

ui_weaponmainwindow.h: weaponmainwindow.ui \
		kqtitleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QMouseEvent
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/uic weaponmainwindow.ui -o ui_weaponmainwindow.h

ui_shipmainwindow.h: shipmainwindow.ui \
		kqtitleframe.h \
		../Qt5.4.1/5.4/clang_64/lib/QtWidgets.framework/Versions/5/Headers/QFrame \
		../Qt5.4.1/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/QTimer \
		../Qt5.4.1/5.4/clang_64/lib/QtGui.framework/Versions/5/Headers/QMouseEvent
	/Users/cf4146/Qt5.4.1/5.4/clang_64/bin/uic shipmainwindow.ui -o ui_shipmainwindow.h

compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 


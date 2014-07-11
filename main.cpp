﻿#include "mainwindow.h"
#include <QApplication>

#include "infomainwindow.h"
#include "timermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("KanPlay");
    a.setWindowIcon(QIcon(QApplication::applicationDirPath()+"/icon.ico"));

    QFont newFont("Meiryo UI", 9);
    a.setFont(newFont);

    QString style = "   \
            QWidget{background-color:rgb(68, 68, 68);}\
            QFrame{border-top:1px solid grey;}\
            QPushButton {   \
                color:white;    \
            }   \
            QPushButton:checked{ background-color: rgb(80, 80, 80); }\
            QPushButton:hover{  \
                background-color: grey; \
                border-style: outset;  \
            }  \
            QTableWidget {selection-background-color: transparent; color:white;}\
            QComboBox{ \
                color: white;   \
                selection-background-color: rgb(68, 68, 68);    \
            }   \
            QComboBox:hover{  \
                background-color: grey; \
                border-style: outset;  \
            }  \
            KQTitleFrame{background-color: rgb(45, 45, 48);}\
            QComboBox QAbstractItemView {   \
                color: white;   \
                selection-background-color: gray;   \
            }\
            ";
    a.setStyleSheet(style);

    MainWindow * w = new MainWindow();

    InfoMainWindow * wInfo = new InfoMainWindow();
    wInfo->setWindowTitle("KanPlay - Info");

    TimerMainWindow * wTimer = new TimerMainWindow();
    wTimer->setWindowTitle("KanPlay - Timer");

    w->postInit(wInfo, wTimer);
    MainWindow::setMainWindow(w);

    w->show();
    wInfo->show();
    wTimer->show();

    return a.exec();
}

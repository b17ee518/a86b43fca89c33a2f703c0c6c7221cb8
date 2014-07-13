#include "mainwindow.h"
#include <QApplication>

#include "infomainwindow.h"
#include "timermainwindow.h"

#include <QJsonDocument>
#include "kandata.h"

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
            QPushButton:checked{\
                background-color: rgb(80, 80, 80);\
                border: none; \
            }\
            QPushButton:hover{  \
                background-color: grey; \
                border-style: outset;  \
            }  \
            QTableWidget {\
                selection-background-color: transparent;\
                color:white;\
                gridline-color: rgb(255, 0, 0);\
            }\
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
            QProgressBar {  \
                background-color: grey;  \
                border: 0px; \
                border-left: 1px solid black;\
                border-radius: 0px; \
            }\
            QProgressBar::chunk {   \
                background-color: grey;  \
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

    //
    /*
    QString str = "";
    QJsonDocument doc = QJsonDocument::fromJson(str.toLocal8Bit());
    KBasicData d;
    d.ReadFromJObj(doc.object()["api_data"].toObject());
    */
    //

    w->show();
    wInfo->show();
    wTimer->show();

    return a.exec();
}

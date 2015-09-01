#include "mainwindow.h"
#include <QApplication>

#include "infomainwindow.h"
#include "timermainwindow.h"
#include "weaponmainwindow.h"

#include <QJsonDocument>
#include "kandata.h"
#include "kanreqdata.h"
#include "kandataconnector.h"
#include <QWinTaskbarProgress>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qsrand(QTime::currentTime().msec());

	a.setApplicationName("KanPlay");
	a.setWindowIcon(QIcon(QApplication::applicationDirPath()+"/icon.ico"));

	QFont newFont("Meiryo UI", 9.5f, QFont::Bold);
	a.setFont(newFont);

	QString style = "   \
			QWidget{background-color:rgb(68, 68, 68);color:white;}\
			QTextEdit{border:none;}\
			QToolTip { color: white; background-color: rgb(80, 80, 80); border: 1px solid black; }\
			QLineEdit{\
				color: rgb(255, 255, 255);\
				border: none;\
			}\
			QFrame{border-top:1px solid grey;}\
			QPushButton:checked{\
				background-color: rgb(80, 80, 80);\
				border: none; \
			}\
			QPushButton:hover{  \
				background-color: rgb(128, 128, 128); \
				border-style: outset;  \
			}  \
			QTableWidget {\
				selection-background-color: transparent;\
				gridline-color: rgb(255, 0, 0);\
			}\
			QComboBox{ \
				selection-background-color: rgb(68, 68, 68);    \
			}   \
			QComboBox:hover{  \
				background-color: grey; \
				border-style: outset;  \
			}  \
			KQTitleFrame{background-color: rgb(45, 45, 48);}\
			QComboBox QAbstractItemView {   \
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
	w->setWindowTitle("KanPlay");

	InfoMainWindow * wInfo = new InfoMainWindow();
	wInfo->setWindowTitle("KanPlay - Info");

	TimerMainWindow * wTimer = new TimerMainWindow();
	wTimer->setWindowTitle("KanPlay - Timer");

	WeaponMainWindow * wWeapon = new WeaponMainWindow();
	wWeapon->setWindowTitle("KanPlay - Weapon");

	ShipMainWindow * wShip = new ShipMainWindow();
	wShip->setWindowTitle("KanPlay - Ship");

    w->postInit(wInfo, wTimer, wWeapon, wShip);
	MainWindow::setMainWindow(w);
	/*
	QFile inputfile(QApplication::applicationDirPath()+"/input.txt");
	inputfile.open(QIODevice::ReadOnly);
	QTextStream in(&inputfile);
	QList<QString> lstargs;
	QString line;
	while (true){
		line = in.readLine();
		if (line.isNull())
		{
			break;
		}
		lstargs.append(line);
	}

	for (int i=0; i<lstargs.count()/3; i++)
	{
        KanDataConnector::getInstance().Parse(lstargs[i*3], lstargs[i*3+1], lstargs[i*3+2]);
	}
	*/
	//

	/*
	w->setAttribute(Qt::WA_DeleteOnClose, true);
	wInfo->setAttribute(Qt::WA_DeleteOnClose, true);
	wTimer->setAttribute(Qt::WA_DeleteOnClose, true);
	wWeapon->setAttribute(Qt::WA_DeleteOnClose, true);
	*/

	w->show();
	wInfo->show();
	wTimer->show();

	return a.exec();
}

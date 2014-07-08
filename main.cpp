#include "kqmainwindow.h"
#include <QApplication>
#include "kqinfosubmainwindow.h"
#include "kqtimersubmainwindow.h"
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("KanPlay");
    a.setWindowIcon(QIcon(QApplication::applicationDirPath()+"/icon.ico"));

    /*
    QRect rc = a.desktop()->availableGeometry(a.desktop()->primaryScreen());
    qDebug("%d,%d,%d,%d", rc.x(), rc.y(), rc.width(), rc.height());
    */

    QFont newFont("Meiryo UI", 9);
    a.setFont(newFont);

    KQMainWindow w;

    KQInfoSubMainWindow wInfo;
    KQTimerSubMainWindow wTimer;
    wInfo.setWindowTitle("KanPlay - Info");
    wTimer.setWindowTitle("KanPlay - Timer");

    w.postInit(&wInfo, &wTimer);
    KQMainWindow::setMainWindow(&w);

    w.setAttribute(Qt::WA_TranslucentBackground, true);
    wInfo.setAttribute(Qt::WA_TranslucentBackground, true);
    wTimer.setAttribute(Qt::WA_TranslucentBackground, true);

//    wInfo.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    w.show();
    wInfo.show();
    wTimer.show();

    w.raise();
    w.activateWindow();

    return a.exec();
}

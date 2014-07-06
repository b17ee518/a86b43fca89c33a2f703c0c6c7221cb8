#include "kqmainwindow.h"
#include <QApplication>
#include "kqinfosubmainwindow.h"
#include "kqtimersubmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont newFont("Meiryo UI", 9);
    a.setFont(newFont);

    KQMainWindow w;

    KQInfoSubMainWindow wInfo;
    KQTimerSubMainWindow wTimer;

    w.postInit(&wInfo, &wTimer);
    KQMainWindow::setMainWindow(&w);


    w.show();
    wInfo.show();
    wTimer.show();

    return a.exec();
}

#ifndef KQUI_MAINTITLEFRAME_H
#define KQUI_MAINTITLEFRAME_H

#include <QWidget>
#include <QFrame>
#include "kqmainwindow.h"
#include "kqinfosubmainwindow.h"
#include "kqtimersubmainwindow.h"
#include "kqmaintitleframe.h"

namespace Ui {
class KQUI_MainTitleFrame;
}

class KQUI_MainTitleFrame : public KQMainTitleFrame
{
    Q_OBJECT

public:
    explicit KQUI_MainTitleFrame(QWidget *parent = 0);
    ~KQUI_MainTitleFrame();

    void postInit();
    void onSubMainWindowShowHide(bool bShow, KQMainWindowBase *pWindow);

private slots:
    void on_pbClose_clicked();

    void on_pbMinimize_clicked();

    void on_pbMinimizeSelf_clicked();

    void on_pbTrasparent_clicked();

    void on_comboBoxZoom_currentIndexChanged(const QString &arg1);

    void on_pbCheckTimer_toggled(bool checked);

    void on_pbCheckInfo_toggled(bool checked);

private:
    Ui::KQUI_MainTitleFrame *ui;

    KQMainWindow * m_pMainWindow;
    KQInfoSubMainWindow * m_pInfoWindow;
    KQTimerSubMainWindow * m_pTimerWindow;
};

#endif // KQUI_MAINTITLEFRAME_H

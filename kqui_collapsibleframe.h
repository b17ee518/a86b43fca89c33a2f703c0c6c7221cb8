#ifndef KQUI_COLLAPSIBLEFRAME_H
#define KQUI_COLLAPSIBLEFRAME_H

#include <QFrame>
#include "kqtablewidget.h"
#include <QPushButton>

namespace Ui {
class KQUI_CollapsibleFrame;
}

class KQUI_CollapsibleFrame : public QFrame
{
    Q_OBJECT

public:
    explicit KQUI_CollapsibleFrame(QWidget *parent = 0);
    ~KQUI_CollapsibleFrame();

    KQTableWidget * tableWidget();
    QPushButton * pushButton();

public slots:
    void slotResize(bool bValue);

private:
    Ui::KQUI_CollapsibleFrame *ui;
};

#endif // KQUI_COLLAPSIBLEFRAME_H

#ifndef KQCOLLAPSIBLETABLEWIDGET_H
#define KQCOLLAPSIBLETABLEWIDGET_H

#include "kqtablewidget.h"
#include "kqpushbutton.h"
#include <QResizeEvent>

class KQCollapsibleTableWidget : public KQTableWidget
{
    Q_OBJECT
public:
    explicit KQCollapsibleTableWidget(QWidget *parent = 0);

protected:

private:
    KQPushButton * m_pCollapseButton;

signals:
    void sigWidthChanged(int w);

public slots:

};

#endif // KQCOLLAPSIBLETABLEWIDGET_H

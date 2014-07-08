#ifndef KQCOLLAPSIBLETABLEWIDGET_H
#define KQCOLLAPSIBLETABLEWIDGET_H

#include "kqtablewidget.h"
#include "kqpushbutton.h"

class KQCollapsibleTableWidget : public KQTableWidget
{
    Q_OBJECT
public:
    explicit KQCollapsibleTableWidget(QWidget *parent = 0);

private:
    KQPushButton * m_pCollapseButton;

signals:

public slots:

};

#endif // KQCOLLAPSIBLETABLEWIDGET_H

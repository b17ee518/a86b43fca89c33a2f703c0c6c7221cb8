#include "kqcollapsibletablewidget.h"
#include "kqpushbutton.h"

KQCollapsibleTableWidget::KQCollapsibleTableWidget(QWidget *parent) :
    KQTableWidget(parent)
{
}

void KQCollapsibleTableWidget::resizeEvent(QResizeEvent *event)
{
    KQTableWidget::resizeEvent(event);
    if (event->oldSize().width() != width())
    {
        emit this->sigWidthChanged(width());
    }
}

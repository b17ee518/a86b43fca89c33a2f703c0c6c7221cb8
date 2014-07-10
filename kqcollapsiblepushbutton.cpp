#include "kqcollapsiblepushbutton.h"

KQCollapsiblePushButton::KQCollapsiblePushButton(QWidget *parent) :
    KQPushButton(parent)
{
}

void KQCollapsiblePushButton::slotOtherSizeChanged(int width)
{
    this->setMinimumWidth(width);
    this->setMaximumWidth(width);
    this->resize(width, this->height());
    qDebug("%d", width);
}

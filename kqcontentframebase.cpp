#include "kqcontentframebase.h"

KQContentFrameBase::KQContentFrameBase(QWidget *parent) :
    KQFrameBase(parent)
{
}

void KQContentFrameBase::resizeEvent(QResizeEvent *event)
{
    KQFrameBase::resizeEvent(event);
    emit this->sigContentFrameSizeChanged(this);
}

#include "kqcontentframebase.h"

KQContentFrameBase::KQContentFrameBase(QWidget *parent) :
    KQFrameBase(parent)
{
    this->setStyleSheet("QWidget{border-top:1px solid grey;}");
}

void KQContentFrameBase::resizeEvent(QResizeEvent *event)
{
    KQFrameBase::resizeEvent(event);
//    emit this->sigContentFrameSizeChanged(this);
}

void KQContentFrameBase::slotHandleContentFrameSizeChange(KQContentFrameBase *p)
{

}

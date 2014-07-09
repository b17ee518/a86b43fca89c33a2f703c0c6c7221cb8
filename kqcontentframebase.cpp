#include "kqcontentframebase.h"

KQContentFrameBase::KQContentFrameBase(QWidget *parent) :
    KQFrameBase(parent)
{
    this->setStyleSheet("QWidget{border-top:1px solid grey;}");
}

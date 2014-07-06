#include "kqpushbuttonbase.h"

KQPushButtonBase::KQPushButtonBase(QWidget *parent) :
    QPushButton(parent)
{
    this->setFlat(true);
    this->setStyleSheet(
                "QWidget{   \
                    color:white;    \
                }   \
                QWidget:hover{  \
                    background-color: grey; \
                    border-style: outset;  \
                }  \
                QPushButton:checked{ background-color: rgb(80, 80, 80); }"
                );
}

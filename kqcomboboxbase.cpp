#include "kqcomboboxbase.h"
#include <qlistview>

KQComboBoxBase::KQComboBoxBase(QWidget *parent) :
    QComboBox(parent)
{
    this->setStyleSheet(
                "QComboBox{ \
                    color: white;   \
                    selection-background-color: rgb(68, 68, 68);    \
                }   \
                QComboBox:hover{  \
                    background-color: grey; \
                    border-style: outset;  \
                }  \
                QComboBox QAbstractItemView {   \
                    color: white;   \
                    selection-background-color: gray;   \
                }"
                );


    QAbstractItemView *abVw = view();
    QListView *listVw = qobject_cast<QListView*>(abVw);

    if (listVw) {
        listVw->setSpacing(2);
        listVw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        listVw->setSizeAdjustPolicy(QListView::AdjustToContents);
        listVw->adjustSize();
    }

}

#include "kqtablewidgetbase.h"
#include <QHeaderView>

KQTableWidgetBase::KQTableWidgetBase(QWidget *parent) :
    QTableWidget(parent)
{
    this->verticalHeader()->setDefaultSectionSize(16);

    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

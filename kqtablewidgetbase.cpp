#include "kqtablewidgetbase.h"
#include <QHeaderView>

KQTableWidgetBase::KQTableWidgetBase(QWidget *parent) :
    QTableWidget(parent)
{
    m_separatorColumn = -1;

    this->verticalHeader()->setDefaultSectionSize(16);

    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setShowGrid(false);
    this->setFocusPolicy(Qt::NoFocus);
    this->setSelectionMode(QAbstractItemView::NoSelection);

    this->setStyleSheet(
                "   \
                QTableWidget {selection-background-color: transparent; color:white;} \
                "
                );
}

void KQTableWidgetBase::setSeparatorColumn(int col)
{
    m_separatorColumn = col;
}

int KQTableWidgetBase::calculateColumnsWidth()
{
    int totalwidth = 0;
    for (int i=0; i<this->columnCount(); i++)
    {
        totalwidth += this->columnWidth(i)+1;
    }
    return totalwidth;
}

void KQTableWidgetBase::slotExpandOnSeparator(int toTotalWidth)
{
    bool bVisible = isVisible();
    show();
    int nowtotalwidth = calculateColumnsWidth();
    if (nowtotalwidth < toTotalWidth)
    {
        if (m_separatorColumn >= 0)
        {
            this->setColumnWidth(m_separatorColumn, this->columnWidth(m_separatorColumn)+toTotalWidth-nowtotalwidth);
        }
    }
/*
    else if (nowtotalwidth > toTotalWidth)
    {
        this->resizeColumnsToContents();
        int newtotalwidth = calculateColumnsWidth();

        if (newtotalwidth > toTotalWidth)
        {
            slotExpandOnSeparator(newtotalwidth);
            emit this->sigExpandOnSeparator(newtotalwidth);
        }
        else
        {
            slotExpandOnSeparator(toTotalWidth);
        }
    }
*/
    setMaximumSize(sizeHint());
    setMinimumSize(sizeHint());
    resize(0, 0);
    if (!bVisible)
    {
        hide();
    }

//    qDebug("m%d",this->maximumWidth());
//    qDebug("t%d",this->width());
}

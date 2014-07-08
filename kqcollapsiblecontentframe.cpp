#include "kqcollapsiblecontentframe.h"
#include <qheaderview.h>

KQCollapsibleContentFrame::KQCollapsibleContentFrame(QWidget *parent) :
    KQContentFrameBase(parent)
{
    m_pCollapseButton = new KQCollapsiblePushButton(this);
    m_pCollapseButton->resize(this->width(), 25);
    m_pCollapseButton->move(0, 0);

    m_pTableWidget = new KQCollapsibleTableWidget(this);
    m_pTableWidget->resize(this->width(), 0);
    m_pTableWidget->move(0, 25);

    this->setStyleSheet("QWidget {border-top:1px solid grey;}");

    connect(m_pCollapseButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleCollapse(bool)));
}

void KQCollapsibleContentFrame::updateRow(QList<KQRowData> rows)
{
    int orowcount = m_pTableWidget->rowCount();

//    m_pTableWidget->setRowCount(0);

    m_pTableWidget->setRowCount(rows.count());
    if (rows.count())
    {
        for (int i=0; i<rows.count(); i++)
        {
            KQRowData * prd = &(rows[i]);

            QList<KQRowCellData> * pRow = prd->cells();
            for (int j=0; j<pRow->count(); j++)
            {
                KQRowCellData * pCell = &((*pRow)[j]);

                QTableWidgetItem * pItem = new QTableWidgetItem(pCell->cellStr());
                m_pTableWidget->setItem(i, j, pItem);
                m_pTableWidget->item(i, j)->setTextColor(pCell->cellCol());
            }
        }
    }

    if (rows.count() != orowcount)
    {
        slotRowCountChanged();
    }
}

void KQCollapsibleContentFrame::resizeEvent(QResizeEvent *event)
{
    KQContentFrameBase::resizeEvent(event);
    m_pCollapseButton->resize(this->width(), m_pCollapseButton->height());
    m_pTableWidget->resize(this->width(), m_pTableWidget->height());
}

void KQCollapsibleContentFrame::slotToggleCollapse(bool bCollapse)
{
    if (bCollapse)
    {
        this->resize(width(), m_pTableWidget->height()+m_pCollapseButton->height());
        // emitted
    }
    else
    {
        this->resize(width(), m_pCollapseButton->height());
        // emitted
    }
}

void KQCollapsibleContentFrame::slotRowCountChanged()
{
    int n = m_pTableWidget->rowCount();
    int height = n*m_pTableWidget->horizontalHeader()->defaultSectionSize();
    m_pTableWidget->resize(m_pTableWidget->width(), height);
    this->resize(this->width(), m_pTableWidget->height()+m_pCollapseButton->height());
}

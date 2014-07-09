#include "kqcollapsiblecontentframe.h"
#include "ui_kqcollapsiblecontentframe.h"


KQCollapsibleContentFrame::KQCollapsibleContentFrame(QWidget *parent) :
    KQContentFrameBase(parent),
    ui(new Ui::KQCollapsibleContentFrame)
{
    ui->setupUi(this);
    m_pCollapseButton = ui->pushButton;
    m_pTableWidget = ui->tableWidget;
    m_pTableWidget->hide();
}

KQCollapsibleContentFrame::~KQCollapsibleContentFrame()
{
    delete ui;
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
            for (int j=0; j<m_pTableWidget->columnCount(); j++)
            {
                KQRowCellData * pCell = &((*pRow)[j]);

                QTableWidgetItem * pItem = new QTableWidgetItem(pCell->cellStr());
                m_pTableWidget->setItem(i, j, pItem);
                m_pTableWidget->item(i, j)->setTextColor(pCell->cellCol());
            }
        }
        m_pTableWidget->setMinimumSize(m_pTableWidget->sizeHint());
        m_pTableWidget->setMaximumSize(m_pTableWidget->sizeHint());
        m_pTableWidget->show();
    }
    else
    {
        m_pTableWidget->hide();
    }

    if (rows.count() != orowcount)
    {
//        slotRowCountChanged();
    }
}

void KQCollapsibleContentFrame::updateCell(int row, int column, KQRowCellData cell)
{
    if (m_pTableWidget->rowCount() > row && m_pTableWidget->columnCount() > column)
    {
        QTableWidgetItem * pItem = m_pTableWidget->item(row, column);
        pItem->setText(cell.cellStr());
        pItem->setTextColor(cell.cellCol());
    }
}

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

    connect(m_pTableWidget, SIGNAL(sigWidthChanged(int)), m_pCollapseButton, SLOT(slotOtherSizeChanged(int)));
}

KQCollapsibleContentFrame::~KQCollapsibleContentFrame()
{
    delete ui;
}

void KQCollapsibleContentFrame::updateRow(QList<KQRowData> rows)
{

    int orowcount = m_pTableWidget->rowCount();

    int towidth = 280;
    int oldwidth = m_pTableWidget->width();

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
//        m_pTableWidget->setMaximumSize(m_pTableWidget->sizeHint());
        m_pTableWidget->resizeColumnsToContents();
        if (m_pTableWidget->calculateColumnsWidth() > towidth)
        {
            towidth = m_pTableWidget->calculateColumnsWidth();
        }
        if (towidth < oldwidth)
        {
            towidth = oldwidth;
        }
        emit m_pTableWidget->sigExpandOnSeparator(towidth);
        m_pTableWidget->slotExpandOnSeparator(towidth);
        m_pTableWidget->show();
    }
    else
    {
        m_pTableWidget->hide();
/*
//        m_pTableWidget->resizeColumnsToContents();

        for (int i=0; i<m_pTableWidget->columnCount(); i++)
        {
            m_pTableWidget->setColumnWidth(i, 1);
        }

        m_pTableWidget->slotExpandOnSeparator(towidth);

//        qDebug("%d", m_pTableWidget->calculateColumnsWidth());
        emit m_pTableWidget->sigExpandOnSeparator(towidth);
*/
    }
    this->window()->adjustSize();
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

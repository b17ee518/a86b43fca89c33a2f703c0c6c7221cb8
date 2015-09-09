#include "kqtablewidget.h"

#include <QHeaderView>
#include <QApplication>

KQTableWidget::KQTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    _separatorColumn = 0;
    setFocusPolicy(Qt::NoFocus);
    setFont(QApplication::font());
}

QSize KQTableWidget::sizeHint() const
{
    return minimumSizeHint();
}

int KQTableWidget::minimumHeight() const
{
    int height = horizontalHeader()->height();
    for (int i = 0; i < rowCount(); ++i)
    {
        height += rowHeight(i)+1;
    }
    return height;
}

int KQTableWidget::minimumWidth() const
{
    int width = 0;
    for (int i = 0; i < columnCount(); ++i)
    {
        width += columnWidth(i)+1;
    }
    return width;
}

void KQTableWidget::adjustSizeToWidth(int width)
{
    int nowtotalwidth = minimumWidth();
    if (nowtotalwidth == width)
    {
    }
    else if (nowtotalwidth < width)
    {
        if (_separatorColumn >= 0)
        {
            this->setColumnWidth(_separatorColumn, this->columnWidth(_separatorColumn)+width-nowtotalwidth);
        }
    }
    else
    {
        resizeColumnsToContents();
        adjustSizeToWidth(width);
    }
    setMinimumSize(sizeHint());
    setMaximumSize(sizeHint());
}

void KQTableWidget::setSeparatorColumn(int n)
{
    _separatorColumn = n;
}

void KQTableWidget::updateFullTable(const QList<KQRowData>& rows)
{
    setRowCount(rows.count());
    for (int i=0; i<rows.count(); i++)
    {
        const KQRowData * prd = &(rows[i]);

        const QList<KQRowCellData> * pRow = prd->cells();
        for (int j=0; j<columnCount(); j++)
        {
            const KQRowCellData * pCell = &((*pRow)[j]);

            QTableWidgetItem * pItem = item(i, j);
            if (!pItem)
            {
                pItem = new QTableWidgetItem();
                setItem(i, j, pItem);
            }
            pItem->setText(pCell->cellStr());
            pItem->setToolTip(pCell->cellToolTip());
            pItem->setTextColor(pCell->cellCol());
        }
    }

    emit this->sigTableSizeChanged();

}

void KQTableWidget::updateCellNoChangeSize(int row, int column, const KQRowCellData& cell)
{
    if (rowCount() > row && columnCount() > column)
    {
        QTableWidgetItem * pItem = item(row, column);
        pItem->setText(cell.cellStr());
        pItem->setTextColor(cell.cellCol());
    }
}

QSize KQTableWidget::minimumSizeHint() const
{
    /*
    QSize size(QTableWidget::sizeHint());
    int width = 0;
    for (int a = 0; a < columnCount(); ++a)
    {
        width += columnWidth(a);
    }
    size.setWidth(width + (columnCount() * 1));
    int height = horizontalHeader()->height();
    for (int a = 0; a < rowCount(); ++a)
    {
        height += rowHeight(a);
    }
    size.setHeight(height + (rowCount() * 1));
    return size;
    */
    return QSize(minimumWidth(), minimumHeight());
}


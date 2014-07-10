#ifndef KQTABLEWIDGETBASE_H
#define KQTABLEWIDGETBASE_H

#include <QTableWidget>
#include "kqwidgetinterface.h"
#include <QHeaderView>

class KQTableWidgetBase : public QTableWidget, public KQWidgetInterface
{
    Q_OBJECT
public:
    explicit KQTableWidgetBase(QWidget *parent = 0);

    QSize KQTableWidgetBase::minimumSizeHint() const
    {
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
    }
    QSize KQTableWidgetBase::sizeHint() const
    {
        return minimumSizeHint();
    }

public:
    void setSeparatorColumn(int col);
    int separatorColumn(){return m_separatorColumn;}
    int calculateColumnsWidth();

private:
    int m_separatorColumn;

signals:
    void sigExpandOnSeparator(int totalWidth);

public slots:
    void slotExpandOnSeparator(int toTotalWidth);

};

#endif // KQTABLEWIDGETBASE_H

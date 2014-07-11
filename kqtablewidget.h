#ifndef KQTABLEWIDGET_H
#define KQTABLEWIDGET_H

#include <QTableWidget>
#include "kqrowdata.h"

class KQTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit KQTableWidget(QWidget *parent = 0);

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

    virtual int minimumHeight() const;
    virtual int minimumWidth() const;

    void adjustSizeToWidth(int width);
    void setSeparatorColumn(int n);

    void updateFullTable(QList<KQRowData> rows);
    void updateCellNoChangeSize(int row, int column, KQRowCellData cell);

signals:

    void sigTableSizeChanged();

public slots:

private:
    int separatorColumn;

};

#endif // KQTABLEWIDGET_H

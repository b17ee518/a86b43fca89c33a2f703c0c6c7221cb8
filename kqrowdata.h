#ifndef KQROWDATA_H
#define KQROWDATA_H

#include <QObject>
#include <QList>
#include <QColor>

class KQRowCellData
{
public:
    explicit KQRowCellData();
    explicit KQRowCellData(QString str);
    explicit KQRowCellData(QString str, QColor col);
    explicit KQRowCellData(QString str, QString tooltip, QColor col);

public:
    void setValue(QString str, QString tooltip, QColor col);
    QString cellStr(){return m_str;}
    QString cellToolTip(){return m_tooltip;}
    QColor cellCol(){return m_col;}

private:
    QString m_str;
    QString m_tooltip;
    QColor m_col;
};

class KQRowData
{
public:
    explicit KQRowData();

    void appendCell(KQRowCellData &celldata);
    void clearCells();
    QList<KQRowCellData> * cells(){return &m_cells;}

private:
    QList<KQRowCellData> m_cells;

};


#endif // KQROWDATA_H

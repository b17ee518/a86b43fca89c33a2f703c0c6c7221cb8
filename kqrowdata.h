#ifndef KQROWDATA_H
#define KQROWDATA_H

#include <QObject>
#include <QList>
#include <QColor>

class KQRowCellData
{
public:
    explicit KQRowCellData();
    explicit KQRowCellData(const QString& str);
    explicit KQRowCellData(const QString& str, const QColor& col);
    explicit KQRowCellData(const QString& str, const QString& tooltip, const QColor& col);

public:
    void setValue(const QString& str, const QString& tooltip, const QColor& col);
    inline const QString& cellStr() const {return _str;}
    inline const QString& cellToolTip() const {return _tooltip;}
    inline const QColor& cellCol() const {return _col;}

private:
    QString _str;
    QString _tooltip;
    QColor _col;
};

class KQRowData
{
public:
    explicit KQRowData();

    void appendCell(const KQRowCellData &celldata);
    void clearCells();
    inline const QList<KQRowCellData> * cells() const {return &_cells;}

private:
    QList<KQRowCellData> _cells;

};


#endif // KQROWDATA_H

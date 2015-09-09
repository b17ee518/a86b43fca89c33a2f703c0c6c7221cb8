#include "kqrowdata.h"

KQRowData::KQRowData()
{
}

void KQRowData::appendCell(const KQRowCellData &celldata)
{
    _cells.append(celldata);
}

void KQRowData::clearCells()
{
    _cells.clear();
}


KQRowCellData::KQRowCellData()
{

}

KQRowCellData::KQRowCellData(const QString& str)
{
    setValue(str, "", QColor(255, 255, 255));
}

KQRowCellData::KQRowCellData(const QString& str, const QColor& col)
{
    setValue(str, "", col);
}

KQRowCellData::KQRowCellData(const QString& str, const QString& tooltip, const QColor& col)
{
    setValue(str, tooltip, col);
}

void KQRowCellData::setValue(const QString& str, const QString& tooltip, const QColor& col)
{
    _str = str;
    _tooltip = tooltip;
    _col = col;
}

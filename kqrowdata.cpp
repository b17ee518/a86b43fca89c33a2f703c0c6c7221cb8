#include "kqrowdata.h"

KQRowData::KQRowData()
{
}

void KQRowData::appendCell(KQRowCellData &celldata)
{
    m_cells.append(celldata);
}

void KQRowData::clearCells()
{
    m_cells.clear();
}


KQRowCellData::KQRowCellData()
{

}

KQRowCellData::KQRowCellData(QString str)
{
    setValue(str, "", QColor(255, 255, 255));
}

KQRowCellData::KQRowCellData(QString str, QColor col)
{
    setValue(str, "", col);
}

KQRowCellData::KQRowCellData(QString str, QString tooltip, QColor col)
{
    setValue(str, tooltip, col);
}

void KQRowCellData::setValue(QString str, QString tooltip, QColor col)
{
    m_str = str;
    m_tooltip = tooltip;
    m_col = col;
}

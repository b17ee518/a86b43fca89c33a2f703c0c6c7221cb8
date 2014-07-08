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
    setValue(str, QColor(255, 255, 255));
}

KQRowCellData::KQRowCellData(QString str, QColor col)
{
    setValue(str, col);
}

void KQRowCellData::setValue(QString str, QColor col)
{
    m_str = str;
    m_col = col;
}

#ifndef KQCOLLAPSIBLECONTENTFRAME_H
#define KQCOLLAPSIBLECONTENTFRAME_H

#include <QWidget>

#include "kqcontentframebase.h"
#include "kqcollapsiblepushbutton.h"
#include "kqcollapsibletablewidget.h"
#include "kqrowdata.h"

namespace Ui {
class KQCollapsibleContentFrame;
}

class KQCollapsibleContentFrame : public KQContentFrameBase
{
    Q_OBJECT

public:
    explicit KQCollapsibleContentFrame(QWidget *parent = 0);
    ~KQCollapsibleContentFrame();

    void updateRow(QList<KQRowData> rows);
    void updateCell(int row, int column, KQRowCellData cell);

private:
    Ui::KQCollapsibleContentFrame *ui;

    KQCollapsiblePushButton * m_pCollapseButton;
    KQCollapsibleTableWidget * m_pTableWidget;

    QList<int> m_lstColMinimumWidth;

public:
    KQCollapsiblePushButton * collapseButton(){return m_pCollapseButton;}
    KQCollapsibleTableWidget * tableWidget(){return m_pTableWidget;}
};

#endif // KQCOLLAPSIBLECONTENTFRAME_H

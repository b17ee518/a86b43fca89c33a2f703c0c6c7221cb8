#ifndef KQCOLLAPSIBLECONTENTFRAME_H
#define KQCOLLAPSIBLECONTENTFRAME_H

#include "kqcontentframebase.h"
#include "kqcollapsiblepushbutton.h"
#include "kqcollapsibletablewidget.h"
#include "kqrowdata.h"
#include <QLayout>

class KQCollapsibleContentFrame : public KQContentFrameBase
{
    Q_OBJECT
public:
    explicit KQCollapsibleContentFrame(QWidget *parent = 0);

    KQCollapsiblePushButton * collapseButton(){return m_pCollapseButton;}
    KQCollapsibleTableWidget * tableWidget(){return m_pTableWidget;}

    void updateRow(QList<KQRowData> rows);

protected:
    virtual void resizeEvent(QResizeEvent *event);

signals:

public slots:

private:
    QVBoxLayout * m_pLayout;
    KQCollapsiblePushButton * m_pCollapseButton;
    KQCollapsibleTableWidget * m_pTableWidget;

protected slots:
    virtual void slotToggleCollapse(bool bCollapse);
    virtual void slotRowCountChanged();


};

#endif // KQCOLLAPSIBLECONTENTFRAME_H

#include "kqcollapsiblecontentframe.h"
#include <qheaderview.h>
#include <QLayout>

KQCollapsibleContentFrame::KQCollapsibleContentFrame(QWidget *parent) :
    KQContentFrameBase(parent)
{
    QWidget* privateLayoutWidget = new QWidget( this );
    privateLayoutWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_pLayout = new QVBoxLayout(privateLayoutWidget);
    m_pLayout->setSpacing(0);
    m_pLayout->setContentsMargins(0, 0, 0, 0);

    m_pCollapseButton = new KQCollapsiblePushButton();
    m_pCollapseButton->resize(this->width(), 25);
    m_pCollapseButton->move(0, 0);
    m_pCollapseButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    m_pTableWidget = new KQCollapsibleTableWidget();
    m_pTableWidget->resize(this->width(), 0);
    m_pTableWidget->move(0, 25);
    m_pTableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_pTableWidget->setMinimumHeight(0);

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setMinimumHeight(25);

    m_pLayout->setSizeConstraint(QLayout::SetMinimumSize);

    m_pLayout->addWidget(m_pCollapseButton);
    m_pLayout->addWidget(m_pTableWidget);

    this->setStyleSheet("QWidget {border-top:1px solid grey;}");

    m_pCollapseButton->show();
    m_pTableWidget->show();

    m_pTableWidget->resize(0, 0);
    m_pTableWidget->adjustSize();
    qDebug("%d",m_pTableWidget->height());
    this->resize(0, 0);
    qDebug("%d", this->minimumHeight());
    this->adjustSize();
    qDebug("%d",height());

    privateLayoutWidget->resize(this->width(), 25);
    privateLayoutWidget->update();

    connect(m_pCollapseButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleCollapse(bool)));
}

void KQCollapsibleContentFrame::updateRow(QList<KQRowData> rows)
{
    int orowcount = m_pTableWidget->rowCount();

//    m_pTableWidget->setRowCount(0);

    m_pTableWidget->setRowCount(rows.count());
    if (rows.count())
    {
        for (int i=0; i<rows.count(); i++)
        {
            KQRowData * prd = &(rows[i]);

            QList<KQRowCellData> * pRow = prd->cells();
            for (int j=0; j<pRow->count(); j++)
            {
                KQRowCellData * pCell = &((*pRow)[j]);

                QTableWidgetItem * pItem = new QTableWidgetItem(pCell->cellStr());
                m_pTableWidget->setItem(i, j, pItem);
                m_pTableWidget->item(i, j)->setTextColor(pCell->cellCol());
            }
        }
    }

    m_pTableWidget->resize(0, 0);
    m_pTableWidget->adjustSize();
    qDebug("%d",m_pTableWidget->height());
    this->setMinimumHeight(25);
    this->resize(0, 0);
    qDebug("%d", this->minimumHeight());

    qDebug(";;%d", m_pLayout->sizeHint().height());

    this->adjustSize();
    qDebug("%d",height());

    this->update();

    if (rows.count() != orowcount)
    {
        slotRowCountChanged();
    }
}

void KQCollapsibleContentFrame::resizeEvent(QResizeEvent *event)
{
    KQContentFrameBase::resizeEvent(event);
//    m_pCollapseButton->resize(this->width(), m_pCollapseButton->height());
//    m_pTableWidget->resize(this->width(), m_pTableWidget->height());
}

void KQCollapsibleContentFrame::slotToggleCollapse(bool bCollapse)
{
    if (bCollapse)
    {
//        this->resize(width(), m_pTableWidget->height()+m_pCollapseButton->height());
        // emitted
    }
    else
    {
//        this->resize(width(), m_pCollapseButton->height());
        // emitted
    }
}

void KQCollapsibleContentFrame::slotRowCountChanged()
{
    int n = m_pTableWidget->rowCount();
    int height = n*m_pTableWidget->horizontalHeader()->defaultSectionSize();
//    m_pTableWidget->resize(m_pTableWidget->width(), height);
//    this->resize(this->width(), m_pTableWidget->height()+m_pCollapseButton->height());
}

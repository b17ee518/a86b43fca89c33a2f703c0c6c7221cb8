#include "kqtitleframe.h"

KQTitleFrame::KQTitleFrame(QWidget *parent) :
    KQFrameBase(parent)
{
    m_bMoving = false;
    m_pHandlingWidget = 0;

    m_pUpdateTimer = new QTimer(this);
    connect(m_pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));

    this->setStyleSheet("background-color: rgb(45, 45, 48);");
}

void KQTitleFrame::setHandlingWidget(QWidget *pw)
{
    m_pHandlingWidget = pw;
}

void KQTitleFrame::stopMoving()
{
    m_bMoving = false;
}

void KQTitleFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bMoving = true;
        m_lastMovingPos = QCursor::pos();
        m_pUpdateTimer->start(16);
    }
    KQFrameBase::mousePressEvent(event);

}

void KQTitleFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_bMoving && event->button() == Qt::LeftButton)
    {
        m_bMoving = false;
    }
    KQFrameBase::mouseReleaseEvent(event);
}

void KQTitleFrame::slotUpdateTimer()
{
    if (!m_bMoving)
    {
        m_pUpdateTimer->stop();
        return;
    }
    QPoint newpos = QCursor::pos();
    m_pHandlingWidget->move(m_pHandlingWidget->pos()+newpos-m_lastMovingPos);
    m_lastMovingPos = newpos;
    QCursor::pos();
}

#include "kqwidgetinterface.h"

KQWidgetInterface::KQWidgetInterface()
{
    m_pWidget = 0;
}

KQWidgetInterface::KQWidgetInterface(QWidget *pw)
{
    wiSetWidget(pw);
}

void KQWidgetInterface::wiSetWidget(QWidget *pw)
{
    m_pWidget = pw;
}

void KQWidgetInterface::wiSetCommonStyleSheet()
{
    if (m_pWidget)
    {
        m_pWidget->setStyleSheet(
                    "background-color:rgb(68, 68, 68);"
                    );
    }
}

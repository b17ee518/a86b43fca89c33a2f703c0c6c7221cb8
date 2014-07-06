#ifndef KQWIDGETINTERFACE_H
#define KQWIDGETINTERFACE_H

#include <QWidget>

class KQWidgetInterface
{
public:
    KQWidgetInterface();
    KQWidgetInterface(QWidget * pw);

public:
    void wiSetWidget(QWidget * pw);

    void wiSetCommonStyleSheet();

private:
    QWidget * m_pWidget;

};

#endif // KQWIDGETINTERFACE_H

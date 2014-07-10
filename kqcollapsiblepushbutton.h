#ifndef KQCOLLAPSIBLEBUTTON_H
#define KQCOLLAPSIBLEBUTTON_H

#include "kqpushbutton.h"

class KQCollapsiblePushButton : public KQPushButton
{
    Q_OBJECT
public:
    explicit KQCollapsiblePushButton(QWidget *parent = 0);

signals:

public slots:
    void slotOtherSizeChanged(int width);
};

#endif // KQCOLLAPSIBLEBUTTON_H

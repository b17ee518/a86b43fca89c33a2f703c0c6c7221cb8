#ifndef KQFRAMEBASE_H
#define KQFRAMEBASE_H

#include <QFrame>
#include "kqwidgetinterface.h"

class KQFrameBase : public QFrame, public KQWidgetInterface
{
    Q_OBJECT
public:
    explicit KQFrameBase(QWidget *parent = 0);

signals:

public slots:

};

#endif // KQFRAMEBASE_H

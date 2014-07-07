#ifndef KQCONTENTFRAMEBASE_H
#define KQCONTENTFRAMEBASE_H

#include "kqframebase.h"

class KQContentFrameBase : public KQFrameBase
{
    Q_OBJECT
public:
    explicit KQContentFrameBase(QWidget *parent = 0);

protected:
    virtual void resizeEvent(QResizeEvent * event);

signals:
    void sigContentFrameSizeChanged(KQContentFrameBase * p);

public slots:

};

#endif // KQCONTENTFRAMEBASE_H

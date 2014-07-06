#ifndef KQTABLEWIDGETBASE_H
#define KQTABLEWIDGETBASE_H

#include <QTableWidget>
#include "kqwidgetinterface.h"

class KQTableWidgetBase : public QTableWidget, public KQWidgetInterface
{
    Q_OBJECT
public:
    explicit KQTableWidgetBase(QWidget *parent = 0);

signals:

public slots:

};

#endif // KQTABLEWIDGETBASE_H

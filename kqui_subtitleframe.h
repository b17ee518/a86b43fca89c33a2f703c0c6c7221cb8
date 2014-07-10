#ifndef KQUI_SUBTITLEFRAME_H
#define KQUI_SUBTITLEFRAME_H

#include <QWidget>
#include "kqtitleframe.h"

namespace Ui {
class KQUI_SubTitleFrame;
}

class KQUI_SubTitleFrame : public KQTitleFrame
{
    Q_OBJECT

public:
    explicit KQUI_SubTitleFrame(QWidget *parent = 0);
    ~KQUI_SubTitleFrame();

    void postInit(QWidget * pHandling);

private slots:
    void on_pbClose_clicked();

    void on_pbMinimize_clicked();

private:
    Ui::KQUI_SubTitleFrame *ui;
};

#endif // KQUI_SUBTITLEFRAME_H

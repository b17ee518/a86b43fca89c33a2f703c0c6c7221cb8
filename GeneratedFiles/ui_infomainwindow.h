/********************************************************************************
** Form generated from reading UI file 'infomainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOMAINWINDOW_H
#define UI_INFOMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "kqtitleframe.h"
#include "kqui_collapsibleframe.h"

QT_BEGIN_NAMESPACE

class Ui_InfoMainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    KQTitleFrame *titleFrame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEditTitle;
    QPushButton *pbMinimize;
    QPushButton *pbClose;
    QFrame *contentFrame;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *overviewTable;
    KQUI_CollapsibleFrame *missionFrame;
    KQUI_CollapsibleFrame *fleetFrame_1;
    KQUI_CollapsibleFrame *fleetFrame_2;
    KQUI_CollapsibleFrame *fleetFrame_3;
    KQUI_CollapsibleFrame *fleetFrame_4;
    KQUI_CollapsibleFrame *repairFrame;

    void setupUi(QMainWindow *InfoMainWindow)
    {
        if (InfoMainWindow->objectName().isEmpty())
            InfoMainWindow->setObjectName(QStringLiteral("InfoMainWindow"));
        InfoMainWindow->resize(320, 369);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(InfoMainWindow->sizePolicy().hasHeightForWidth());
        InfoMainWindow->setSizePolicy(sizePolicy);
        InfoMainWindow->setMinimumSize(QSize(320, 224));
        centralwidget = new QWidget(InfoMainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        titleFrame = new KQTitleFrame(centralwidget);
        titleFrame->setObjectName(QStringLiteral("titleFrame"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(titleFrame->sizePolicy().hasHeightForWidth());
        titleFrame->setSizePolicy(sizePolicy1);
        titleFrame->setMinimumSize(QSize(280, 20));
        titleFrame->setMaximumSize(QSize(16777215, 20));
        titleFrame->setFrameShape(QFrame::NoFrame);
        titleFrame->setFrameShadow(QFrame::Plain);
        titleFrame->setLineWidth(0);
        horizontalLayout = new QHBoxLayout(titleFrame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEditTitle = new QLineEdit(titleFrame);
        lineEditTitle->setObjectName(QStringLiteral("lineEditTitle"));
        lineEditTitle->setReadOnly(true);

        horizontalLayout->addWidget(lineEditTitle);

        pbMinimize = new QPushButton(titleFrame);
        pbMinimize->setObjectName(QStringLiteral("pbMinimize"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pbMinimize->sizePolicy().hasHeightForWidth());
        pbMinimize->setSizePolicy(sizePolicy2);
        pbMinimize->setMinimumSize(QSize(20, 20));
        pbMinimize->setMaximumSize(QSize(20, 20));
        pbMinimize->setFlat(true);

        horizontalLayout->addWidget(pbMinimize);

        pbClose = new QPushButton(titleFrame);
        pbClose->setObjectName(QStringLiteral("pbClose"));
        sizePolicy2.setHeightForWidth(pbClose->sizePolicy().hasHeightForWidth());
        pbClose->setSizePolicy(sizePolicy2);
        pbClose->setMinimumSize(QSize(20, 20));
        pbClose->setMaximumSize(QSize(20, 20));
        pbClose->setFlat(true);

        horizontalLayout->addWidget(pbClose);


        verticalLayout->addWidget(titleFrame);

        contentFrame = new QFrame(centralwidget);
        contentFrame->setObjectName(QStringLiteral("contentFrame"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(contentFrame->sizePolicy().hasHeightForWidth());
        contentFrame->setSizePolicy(sizePolicy3);
        contentFrame->setFrameShape(QFrame::NoFrame);
        contentFrame->setFrameShadow(QFrame::Plain);
        contentFrame->setLineWidth(0);
        verticalLayout_2 = new QVBoxLayout(contentFrame);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        overviewTable = new QTableWidget(contentFrame);
        overviewTable->setObjectName(QStringLiteral("overviewTable"));
        sizePolicy1.setHeightForWidth(overviewTable->sizePolicy().hasHeightForWidth());
        overviewTable->setSizePolicy(sizePolicy1);
        overviewTable->setMinimumSize(QSize(320, 54));
        overviewTable->setMaximumSize(QSize(16777215, 54));
        overviewTable->setFrameShape(QFrame::NoFrame);
        overviewTable->setFrameShadow(QFrame::Plain);
        overviewTable->setLineWidth(0);
        overviewTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        overviewTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        overviewTable->setAutoScroll(false);
        overviewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        overviewTable->setSelectionMode(QAbstractItemView::NoSelection);
        overviewTable->setShowGrid(false);
        overviewTable->setGridStyle(Qt::NoPen);
        overviewTable->setCornerButtonEnabled(false);
        overviewTable->horizontalHeader()->setVisible(false);
        overviewTable->horizontalHeader()->setDefaultSectionSize(18);
        overviewTable->horizontalHeader()->setHighlightSections(false);
        overviewTable->horizontalHeader()->setMinimumSectionSize(18);
        overviewTable->verticalHeader()->setVisible(false);
        overviewTable->verticalHeader()->setDefaultSectionSize(16);
        overviewTable->verticalHeader()->setHighlightSections(false);
        overviewTable->verticalHeader()->setMinimumSectionSize(16);

        verticalLayout_2->addWidget(overviewTable);

        missionFrame = new KQUI_CollapsibleFrame(contentFrame);
        missionFrame->setObjectName(QStringLiteral("missionFrame"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(missionFrame->sizePolicy().hasHeightForWidth());
        missionFrame->setSizePolicy(sizePolicy4);
        missionFrame->setMinimumSize(QSize(0, 25));
        missionFrame->setFrameShape(QFrame::StyledPanel);
        missionFrame->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(missionFrame);

        fleetFrame_1 = new KQUI_CollapsibleFrame(contentFrame);
        fleetFrame_1->setObjectName(QStringLiteral("fleetFrame_1"));
        sizePolicy4.setHeightForWidth(fleetFrame_1->sizePolicy().hasHeightForWidth());
        fleetFrame_1->setSizePolicy(sizePolicy4);
        fleetFrame_1->setMinimumSize(QSize(0, 25));
        fleetFrame_1->setFrameShape(QFrame::StyledPanel);
        fleetFrame_1->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(fleetFrame_1);

        fleetFrame_2 = new KQUI_CollapsibleFrame(contentFrame);
        fleetFrame_2->setObjectName(QStringLiteral("fleetFrame_2"));
        sizePolicy4.setHeightForWidth(fleetFrame_2->sizePolicy().hasHeightForWidth());
        fleetFrame_2->setSizePolicy(sizePolicy4);
        fleetFrame_2->setMinimumSize(QSize(0, 25));
        fleetFrame_2->setFrameShape(QFrame::StyledPanel);
        fleetFrame_2->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(fleetFrame_2);

        fleetFrame_3 = new KQUI_CollapsibleFrame(contentFrame);
        fleetFrame_3->setObjectName(QStringLiteral("fleetFrame_3"));
        sizePolicy4.setHeightForWidth(fleetFrame_3->sizePolicy().hasHeightForWidth());
        fleetFrame_3->setSizePolicy(sizePolicy4);
        fleetFrame_3->setMinimumSize(QSize(0, 25));
        fleetFrame_3->setFrameShape(QFrame::StyledPanel);
        fleetFrame_3->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(fleetFrame_3);

        fleetFrame_4 = new KQUI_CollapsibleFrame(contentFrame);
        fleetFrame_4->setObjectName(QStringLiteral("fleetFrame_4"));
        sizePolicy4.setHeightForWidth(fleetFrame_4->sizePolicy().hasHeightForWidth());
        fleetFrame_4->setSizePolicy(sizePolicy4);
        fleetFrame_4->setMinimumSize(QSize(0, 25));
        fleetFrame_4->setFrameShape(QFrame::StyledPanel);
        fleetFrame_4->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(fleetFrame_4);

        repairFrame = new KQUI_CollapsibleFrame(contentFrame);
        repairFrame->setObjectName(QStringLiteral("repairFrame"));
        sizePolicy4.setHeightForWidth(repairFrame->sizePolicy().hasHeightForWidth());
        repairFrame->setSizePolicy(sizePolicy4);
        repairFrame->setMinimumSize(QSize(0, 25));
        repairFrame->setFrameShape(QFrame::StyledPanel);
        repairFrame->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(repairFrame);


        verticalLayout->addWidget(contentFrame);

        InfoMainWindow->setCentralWidget(centralwidget);

        retranslateUi(InfoMainWindow);

        QMetaObject::connectSlotsByName(InfoMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *InfoMainWindow)
    {
        InfoMainWindow->setWindowTitle(QApplication::translate("InfoMainWindow", "MainWindow", 0));
        pbMinimize->setText(QApplication::translate("InfoMainWindow", "-", 0));
        pbClose->setText(QApplication::translate("InfoMainWindow", "X", 0));
    } // retranslateUi

};

namespace Ui {
    class InfoMainWindow: public Ui_InfoMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOMAINWINDOW_H

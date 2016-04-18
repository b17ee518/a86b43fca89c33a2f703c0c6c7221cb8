/********************************************************************************
** Form generated from reading UI file 'timermainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMERMAINWINDOW_H
#define UI_TIMERMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "kqtitleframe.h"

QT_BEGIN_NAMESPACE

class Ui_TimerMainWindow
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
    QTableWidget *expeditionTable;
    QTableWidget *repairTable;
    QTableWidget *buildTable;
    QFrame *progressFrame;
    QHBoxLayout *horizontalLayout_2;
    QProgressBar *progressExp1u;
    QProgressBar *progressExp2u;
    QProgressBar *progressExp3u;
    QSpacerItem *horizontalSpacer_2;
    QProgressBar *progressRepair1u;
    QProgressBar *progressRepair2u;
    QProgressBar *progressRepair3u;
    QProgressBar *progressRepair4u;

    void setupUi(QMainWindow *TimerMainWindow)
    {
        if (TimerMainWindow->objectName().isEmpty())
            TimerMainWindow->setObjectName(QStringLiteral("TimerMainWindow"));
        TimerMainWindow->resize(256, 345);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TimerMainWindow->sizePolicy().hasHeightForWidth());
        TimerMainWindow->setSizePolicy(sizePolicy);
        TimerMainWindow->setMinimumSize(QSize(256, 345));
        TimerMainWindow->setMaximumSize(QSize(256, 345));
        centralwidget = new QWidget(TimerMainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        titleFrame = new KQTitleFrame(centralwidget);
        titleFrame->setObjectName(QStringLiteral("titleFrame"));
        sizePolicy.setHeightForWidth(titleFrame->sizePolicy().hasHeightForWidth());
        titleFrame->setSizePolicy(sizePolicy);
        titleFrame->setMinimumSize(QSize(256, 20));
        titleFrame->setMaximumSize(QSize(256, 20));
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
        sizePolicy.setHeightForWidth(pbMinimize->sizePolicy().hasHeightForWidth());
        pbMinimize->setSizePolicy(sizePolicy);
        pbMinimize->setMinimumSize(QSize(20, 20));
        pbMinimize->setMaximumSize(QSize(20, 20));
        pbMinimize->setFlat(true);

        horizontalLayout->addWidget(pbMinimize);

        pbClose = new QPushButton(titleFrame);
        pbClose->setObjectName(QStringLiteral("pbClose"));
        sizePolicy.setHeightForWidth(pbClose->sizePolicy().hasHeightForWidth());
        pbClose->setSizePolicy(sizePolicy);
        pbClose->setMinimumSize(QSize(20, 20));
        pbClose->setMaximumSize(QSize(20, 20));
        pbClose->setFlat(true);

        horizontalLayout->addWidget(pbClose);


        verticalLayout->addWidget(titleFrame);

        expeditionTable = new QTableWidget(centralwidget);
        if (expeditionTable->columnCount() < 3)
            expeditionTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        expeditionTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        expeditionTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        expeditionTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (expeditionTable->rowCount() < 6)
            expeditionTable->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        expeditionTable->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        expeditionTable->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        expeditionTable->setVerticalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        expeditionTable->setVerticalHeaderItem(3, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        expeditionTable->setVerticalHeaderItem(4, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        expeditionTable->setVerticalHeaderItem(5, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        __qtablewidgetitem9->setFlags(Qt::ItemIsEnabled);
        expeditionTable->setItem(0, 1, __qtablewidgetitem9);
        expeditionTable->setObjectName(QStringLiteral("expeditionTable"));
        sizePolicy.setHeightForWidth(expeditionTable->sizePolicy().hasHeightForWidth());
        expeditionTable->setSizePolicy(sizePolicy);
        expeditionTable->setMinimumSize(QSize(0, 120));
        expeditionTable->setMaximumSize(QSize(16777215, 120));
        expeditionTable->setFrameShape(QFrame::NoFrame);
        expeditionTable->setFrameShadow(QFrame::Plain);
        expeditionTable->setLineWidth(0);
        expeditionTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        expeditionTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        expeditionTable->setAutoScroll(false);
        expeditionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        expeditionTable->setSelectionMode(QAbstractItemView::NoSelection);
        expeditionTable->setShowGrid(false);
        expeditionTable->setCornerButtonEnabled(false);
        expeditionTable->horizontalHeader()->setVisible(false);
        expeditionTable->horizontalHeader()->setDefaultSectionSize(18);
        expeditionTable->horizontalHeader()->setHighlightSections(false);
        expeditionTable->horizontalHeader()->setMinimumSectionSize(18);
        expeditionTable->verticalHeader()->setVisible(false);
        expeditionTable->verticalHeader()->setDefaultSectionSize(16);
        expeditionTable->verticalHeader()->setHighlightSections(false);
        expeditionTable->verticalHeader()->setMinimumSectionSize(16);

        verticalLayout->addWidget(expeditionTable);

        repairTable = new QTableWidget(centralwidget);
        if (repairTable->columnCount() < 3)
            repairTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        repairTable->setHorizontalHeaderItem(0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        repairTable->setHorizontalHeaderItem(1, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        repairTable->setHorizontalHeaderItem(2, __qtablewidgetitem12);
        if (repairTable->rowCount() < 8)
            repairTable->setRowCount(8);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(2, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(3, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(4, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(5, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(6, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        repairTable->setVerticalHeaderItem(7, __qtablewidgetitem20);
        repairTable->setObjectName(QStringLiteral("repairTable"));
        sizePolicy.setHeightForWidth(repairTable->sizePolicy().hasHeightForWidth());
        repairTable->setSizePolicy(sizePolicy);
        repairTable->setMinimumSize(QSize(0, 160));
        repairTable->setMaximumSize(QSize(16777215, 160));
        QFont font;
        font.setFamily(QStringLiteral("Meiryo UI"));
        font.setPointSize(11);
        font.setBold(false);
        font.setWeight(50);
        repairTable->setFont(font);
        repairTable->setFrameShape(QFrame::NoFrame);
        repairTable->setFrameShadow(QFrame::Plain);
        repairTable->setLineWidth(0);
        repairTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        repairTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        repairTable->setAutoScroll(false);
        repairTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        repairTable->setSelectionMode(QAbstractItemView::NoSelection);
        repairTable->setShowGrid(false);
        repairTable->setCornerButtonEnabled(false);
        repairTable->horizontalHeader()->setVisible(false);
        repairTable->horizontalHeader()->setDefaultSectionSize(18);
        repairTable->horizontalHeader()->setHighlightSections(false);
        repairTable->horizontalHeader()->setMinimumSectionSize(18);
        repairTable->verticalHeader()->setVisible(false);
        repairTable->verticalHeader()->setDefaultSectionSize(16);
        repairTable->verticalHeader()->setHighlightSections(false);
        repairTable->verticalHeader()->setMinimumSectionSize(16);

        verticalLayout->addWidget(repairTable);

        buildTable = new QTableWidget(centralwidget);
        if (buildTable->columnCount() < 3)
            buildTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        buildTable->setHorizontalHeaderItem(0, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        buildTable->setHorizontalHeaderItem(1, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        buildTable->setHorizontalHeaderItem(2, __qtablewidgetitem23);
        if (buildTable->rowCount() < 2)
            buildTable->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        buildTable->setVerticalHeaderItem(0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        buildTable->setVerticalHeaderItem(1, __qtablewidgetitem25);
        buildTable->setObjectName(QStringLiteral("buildTable"));
        sizePolicy.setHeightForWidth(buildTable->sizePolicy().hasHeightForWidth());
        buildTable->setSizePolicy(sizePolicy);
        buildTable->setMinimumSize(QSize(0, 40));
        buildTable->setMaximumSize(QSize(16777215, 40));
        buildTable->setFrameShape(QFrame::NoFrame);
        buildTable->setFrameShadow(QFrame::Plain);
        buildTable->setLineWidth(0);
        buildTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        buildTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        buildTable->setAutoScroll(false);
        buildTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        buildTable->setSelectionMode(QAbstractItemView::NoSelection);
        buildTable->setShowGrid(false);
        buildTable->setCornerButtonEnabled(false);
        buildTable->horizontalHeader()->setVisible(false);
        buildTable->horizontalHeader()->setDefaultSectionSize(16);
        buildTable->horizontalHeader()->setHighlightSections(false);
        buildTable->horizontalHeader()->setMinimumSectionSize(16);
        buildTable->verticalHeader()->setVisible(false);
        buildTable->verticalHeader()->setDefaultSectionSize(16);
        buildTable->verticalHeader()->setHighlightSections(false);
        buildTable->verticalHeader()->setMinimumSectionSize(16);

        verticalLayout->addWidget(buildTable);

        progressFrame = new QFrame(centralwidget);
        progressFrame->setObjectName(QStringLiteral("progressFrame"));
        sizePolicy.setHeightForWidth(progressFrame->sizePolicy().hasHeightForWidth());
        progressFrame->setSizePolicy(sizePolicy);
        progressFrame->setMinimumSize(QSize(0, 5));
        progressFrame->setMaximumSize(QSize(256, 5));
        progressFrame->setFrameShape(QFrame::NoFrame);
        progressFrame->setFrameShadow(QFrame::Plain);
        progressFrame->setLineWidth(0);
        horizontalLayout_2 = new QHBoxLayout(progressFrame);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        progressExp1u = new QProgressBar(progressFrame);
        progressExp1u->setObjectName(QStringLiteral("progressExp1u"));
        progressExp1u->setMinimumSize(QSize(0, 5));
        progressExp1u->setMaximumSize(QSize(16777215, 5));
        progressExp1u->setValue(24);
        progressExp1u->setTextVisible(false);

        horizontalLayout_2->addWidget(progressExp1u);

        progressExp2u = new QProgressBar(progressFrame);
        progressExp2u->setObjectName(QStringLiteral("progressExp2u"));
        progressExp2u->setMinimumSize(QSize(0, 5));
        progressExp2u->setMaximumSize(QSize(16777215, 5));
        progressExp2u->setValue(24);
        progressExp2u->setTextVisible(false);

        horizontalLayout_2->addWidget(progressExp2u);

        progressExp3u = new QProgressBar(progressFrame);
        progressExp3u->setObjectName(QStringLiteral("progressExp3u"));
        progressExp3u->setMinimumSize(QSize(0, 5));
        progressExp3u->setMaximumSize(QSize(16777215, 5));
        progressExp3u->setValue(24);
        progressExp3u->setTextVisible(false);

        horizontalLayout_2->addWidget(progressExp3u);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        progressRepair1u = new QProgressBar(progressFrame);
        progressRepair1u->setObjectName(QStringLiteral("progressRepair1u"));
        progressRepair1u->setMinimumSize(QSize(0, 5));
        progressRepair1u->setMaximumSize(QSize(16777215, 5));
        progressRepair1u->setValue(24);
        progressRepair1u->setTextVisible(false);

        horizontalLayout_2->addWidget(progressRepair1u);

        progressRepair2u = new QProgressBar(progressFrame);
        progressRepair2u->setObjectName(QStringLiteral("progressRepair2u"));
        progressRepair2u->setMinimumSize(QSize(0, 5));
        progressRepair2u->setMaximumSize(QSize(16777215, 5));
        progressRepair2u->setValue(24);
        progressRepair2u->setTextVisible(false);

        horizontalLayout_2->addWidget(progressRepair2u);

        progressRepair3u = new QProgressBar(progressFrame);
        progressRepair3u->setObjectName(QStringLiteral("progressRepair3u"));
        progressRepair3u->setMinimumSize(QSize(0, 5));
        progressRepair3u->setMaximumSize(QSize(16777215, 5));
        progressRepair3u->setValue(24);
        progressRepair3u->setTextVisible(false);

        horizontalLayout_2->addWidget(progressRepair3u);

        progressRepair4u = new QProgressBar(progressFrame);
        progressRepair4u->setObjectName(QStringLiteral("progressRepair4u"));
        progressRepair4u->setMinimumSize(QSize(0, 5));
        progressRepair4u->setMaximumSize(QSize(16777215, 5));
        progressRepair4u->setValue(24);
        progressRepair4u->setTextVisible(false);

        horizontalLayout_2->addWidget(progressRepair4u);


        verticalLayout->addWidget(progressFrame);

        TimerMainWindow->setCentralWidget(centralwidget);

        retranslateUi(TimerMainWindow);

        QMetaObject::connectSlotsByName(TimerMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TimerMainWindow)
    {
        TimerMainWindow->setWindowTitle(QApplication::translate("TimerMainWindow", "MainWindow", 0));
        pbMinimize->setText(QApplication::translate("TimerMainWindow", "-", 0));
        pbClose->setText(QApplication::translate("TimerMainWindow", "X", 0));
        QTableWidgetItem *___qtablewidgetitem = expeditionTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("TimerMainWindow", "\357\274\203", 0));
        QTableWidgetItem *___qtablewidgetitem1 = expeditionTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("TimerMainWindow", "Timer", 0));
        QTableWidgetItem *___qtablewidgetitem2 = expeditionTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("TimerMainWindow", "ExpTime", 0));
        QTableWidgetItem *___qtablewidgetitem3 = expeditionTable->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("TimerMainWindow", "\342\221\240T", 0));
        QTableWidgetItem *___qtablewidgetitem4 = expeditionTable->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("TimerMainWindow", "\342\221\240N", 0));
        QTableWidgetItem *___qtablewidgetitem5 = expeditionTable->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("TimerMainWindow", "\342\221\241T", 0));
        QTableWidgetItem *___qtablewidgetitem6 = expeditionTable->verticalHeaderItem(3);
        ___qtablewidgetitem6->setText(QApplication::translate("TimerMainWindow", "\342\221\241N", 0));
        QTableWidgetItem *___qtablewidgetitem7 = expeditionTable->verticalHeaderItem(4);
        ___qtablewidgetitem7->setText(QApplication::translate("TimerMainWindow", "\342\221\242T", 0));
        QTableWidgetItem *___qtablewidgetitem8 = expeditionTable->verticalHeaderItem(5);
        ___qtablewidgetitem8->setText(QApplication::translate("TimerMainWindow", "\342\221\242N", 0));

        const bool __sortingEnabled = expeditionTable->isSortingEnabled();
        expeditionTable->setSortingEnabled(false);
        expeditionTable->setSortingEnabled(__sortingEnabled);

        QTableWidgetItem *___qtablewidgetitem9 = repairTable->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("TimerMainWindow", "\357\274\203", 0));
        QTableWidgetItem *___qtablewidgetitem10 = repairTable->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("TimerMainWindow", "Timer", 0));
        QTableWidgetItem *___qtablewidgetitem11 = repairTable->horizontalHeaderItem(2);
        ___qtablewidgetitem11->setText(QApplication::translate("TimerMainWindow", "ExpTime", 0));
        QTableWidgetItem *___qtablewidgetitem12 = repairTable->verticalHeaderItem(0);
        ___qtablewidgetitem12->setText(QApplication::translate("TimerMainWindow", "\342\221\240T", 0));
        QTableWidgetItem *___qtablewidgetitem13 = repairTable->verticalHeaderItem(1);
        ___qtablewidgetitem13->setText(QApplication::translate("TimerMainWindow", "\342\221\240N", 0));
        QTableWidgetItem *___qtablewidgetitem14 = repairTable->verticalHeaderItem(2);
        ___qtablewidgetitem14->setText(QApplication::translate("TimerMainWindow", "\342\221\241T", 0));
        QTableWidgetItem *___qtablewidgetitem15 = repairTable->verticalHeaderItem(3);
        ___qtablewidgetitem15->setText(QApplication::translate("TimerMainWindow", "\342\221\241N", 0));
        QTableWidgetItem *___qtablewidgetitem16 = repairTable->verticalHeaderItem(4);
        ___qtablewidgetitem16->setText(QApplication::translate("TimerMainWindow", "\342\221\242T", 0));
        QTableWidgetItem *___qtablewidgetitem17 = repairTable->verticalHeaderItem(5);
        ___qtablewidgetitem17->setText(QApplication::translate("TimerMainWindow", "\342\221\242N", 0));
        QTableWidgetItem *___qtablewidgetitem18 = repairTable->verticalHeaderItem(6);
        ___qtablewidgetitem18->setText(QApplication::translate("TimerMainWindow", "\342\221\243T", 0));
        QTableWidgetItem *___qtablewidgetitem19 = repairTable->verticalHeaderItem(7);
        ___qtablewidgetitem19->setText(QApplication::translate("TimerMainWindow", "\342\221\243N", 0));
        QTableWidgetItem *___qtablewidgetitem20 = buildTable->horizontalHeaderItem(0);
        ___qtablewidgetitem20->setText(QApplication::translate("TimerMainWindow", "\357\274\203", 0));
        QTableWidgetItem *___qtablewidgetitem21 = buildTable->horizontalHeaderItem(1);
        ___qtablewidgetitem21->setText(QApplication::translate("TimerMainWindow", "Timer", 0));
        QTableWidgetItem *___qtablewidgetitem22 = buildTable->horizontalHeaderItem(2);
        ___qtablewidgetitem22->setText(QApplication::translate("TimerMainWindow", "Name", 0));
        QTableWidgetItem *___qtablewidgetitem23 = buildTable->verticalHeaderItem(0);
        ___qtablewidgetitem23->setText(QApplication::translate("TimerMainWindow", "\342\221\240", 0));
        QTableWidgetItem *___qtablewidgetitem24 = buildTable->verticalHeaderItem(1);
        ___qtablewidgetitem24->setText(QApplication::translate("TimerMainWindow", "\342\221\241", 0));
    } // retranslateUi

};

namespace Ui {
    class TimerMainWindow: public Ui_TimerMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMERMAINWINDOW_H

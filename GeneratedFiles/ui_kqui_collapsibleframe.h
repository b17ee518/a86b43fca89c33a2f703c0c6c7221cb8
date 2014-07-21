/********************************************************************************
** Form generated from reading UI file 'kqui_collapsibleframe.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KQUI_COLLAPSIBLEFRAME_H
#define UI_KQUI_COLLAPSIBLEFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "kqtablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_KQUI_CollapsibleFrame
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    KQTableWidget *tableWidget;

    void setupUi(QWidget *KQUI_CollapsibleFrame)
    {
        if (KQUI_CollapsibleFrame->objectName().isEmpty())
            KQUI_CollapsibleFrame->setObjectName(QStringLiteral("KQUI_CollapsibleFrame"));
        KQUI_CollapsibleFrame->resize(400, 300);
        gridLayout = new QGridLayout(KQUI_CollapsibleFrame);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(KQUI_CollapsibleFrame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(280, 25));
        pushButton->setMaximumSize(QSize(16777215, 25));
        pushButton->setCheckable(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        tableWidget = new KQTableWidget(KQUI_CollapsibleFrame);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMinimumSize(QSize(280, 0));
        tableWidget->setFrameShape(QFrame::NoFrame);
        tableWidget->setFrameShadow(QFrame::Plain);
        tableWidget->setLineWidth(0);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setAutoScroll(false);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget->setShowGrid(false);
        tableWidget->setGridStyle(Qt::NoPen);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(18);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->horizontalHeader()->setMinimumSectionSize(18);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setDefaultSectionSize(16);
        tableWidget->verticalHeader()->setHighlightSections(false);
        tableWidget->verticalHeader()->setMinimumSectionSize(16);

        gridLayout->addWidget(tableWidget, 1, 0, 1, 2);


        retranslateUi(KQUI_CollapsibleFrame);
        QObject::connect(pushButton, SIGNAL(toggled(bool)), tableWidget, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(KQUI_CollapsibleFrame);
    } // setupUi

    void retranslateUi(QWidget *KQUI_CollapsibleFrame)
    {
        KQUI_CollapsibleFrame->setWindowTitle(QApplication::translate("KQUI_CollapsibleFrame", "Form", 0));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class KQUI_CollapsibleFrame: public Ui_KQUI_CollapsibleFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KQUI_COLLAPSIBLEFRAME_H

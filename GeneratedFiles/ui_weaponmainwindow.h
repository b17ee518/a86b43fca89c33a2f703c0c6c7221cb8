/********************************************************************************
** Form generated from reading UI file 'weaponmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEAPONMAINWINDOW_H
#define UI_WEAPONMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "kqtitleframe.h"

QT_BEGIN_NAMESPACE

class Ui_WeaponMainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    KQTitleFrame *titleFrame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEditTitle;
    QPushButton *pbMinimize;
    QPushButton *pbClose;
    QFrame *contentFrame;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;

    void setupUi(QMainWindow *WeaponMainWindow)
    {
        if (WeaponMainWindow->objectName().isEmpty())
            WeaponMainWindow->setObjectName(QStringLiteral("WeaponMainWindow"));
        WeaponMainWindow->resize(320, 369);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WeaponMainWindow->sizePolicy().hasHeightForWidth());
        WeaponMainWindow->setSizePolicy(sizePolicy);
        WeaponMainWindow->setMinimumSize(QSize(320, 440));
        centralwidget = new QWidget(WeaponMainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
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
        horizontalSpacer = new QSpacerItem(60, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEditTitle = new QLineEdit(titleFrame);
        lineEditTitle->setObjectName(QStringLiteral("lineEditTitle"));
        lineEditTitle->setReadOnly(false);

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


        gridLayout->addWidget(titleFrame, 0, 0, 1, 2);

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
        scrollArea = new QScrollArea(contentFrame);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 318, 347));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);


        gridLayout->addWidget(contentFrame, 1, 0, 1, 1);

        WeaponMainWindow->setCentralWidget(centralwidget);

        retranslateUi(WeaponMainWindow);

        QMetaObject::connectSlotsByName(WeaponMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *WeaponMainWindow)
    {
        WeaponMainWindow->setWindowTitle(QApplication::translate("WeaponMainWindow", "MainWindow", 0));
        pbMinimize->setText(QApplication::translate("WeaponMainWindow", "-", 0));
        pbClose->setText(QApplication::translate("WeaponMainWindow", "X", 0));
    } // retranslateUi

};

namespace Ui {
    class WeaponMainWindow: public Ui_WeaponMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEAPONMAINWINDOW_H

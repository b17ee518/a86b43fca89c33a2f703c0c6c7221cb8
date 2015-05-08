/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "kqtitleframe.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    KQTitleFrame *titleFrame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbRefresh;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pbScreenshot;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pbSwitchScreenshot;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbCheckLowVol;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pbCheckInfo;
    QPushButton *pbCheckTimer;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *pbCheckWeapon;
    QSpacerItem *horizontalSpacer_5;
    QComboBox *comboBoxZoom;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pbMoveTogether;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pbCheckTrasparent;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pbMinimize;
    QPushButton *pbClose;
    QFrame *webFrame;
    QVBoxLayout *verticalLayout_2;
    QWebView *webView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 500);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        titleFrame = new KQTitleFrame(centralWidget);
        titleFrame->setObjectName(QStringLiteral("titleFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(titleFrame->sizePolicy().hasHeightForWidth());
        titleFrame->setSizePolicy(sizePolicy);
        titleFrame->setMinimumSize(QSize(0, 20));
        titleFrame->setMaximumSize(QSize(16777215, 20));
        titleFrame->setFrameShape(QFrame::NoFrame);
        titleFrame->setFrameShadow(QFrame::Plain);
        titleFrame->setLineWidth(0);
        horizontalLayout = new QHBoxLayout(titleFrame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(16, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pbRefresh = new QPushButton(titleFrame);
        pbRefresh->setObjectName(QStringLiteral("pbRefresh"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pbRefresh->sizePolicy().hasHeightForWidth());
        pbRefresh->setSizePolicy(sizePolicy1);
        pbRefresh->setMinimumSize(QSize(20, 20));
        pbRefresh->setMaximumSize(QSize(20, 20));
        pbRefresh->setFlat(true);

        horizontalLayout->addWidget(pbRefresh);

        horizontalSpacer_8 = new QSpacerItem(120, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);

        pbScreenshot = new QPushButton(titleFrame);
        pbScreenshot->setObjectName(QStringLiteral("pbScreenshot"));
        sizePolicy1.setHeightForWidth(pbScreenshot->sizePolicy().hasHeightForWidth());
        pbScreenshot->setSizePolicy(sizePolicy1);
        pbScreenshot->setMinimumSize(QSize(20, 20));
        pbScreenshot->setMaximumSize(QSize(20, 20));
        pbScreenshot->setFlat(true);

        horizontalLayout->addWidget(pbScreenshot);

        horizontalSpacer_9 = new QSpacerItem(16, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);

        pbSwitchScreenshot = new QPushButton(titleFrame);
        pbSwitchScreenshot->setObjectName(QStringLiteral("pbSwitchScreenshot"));
        sizePolicy1.setHeightForWidth(pbSwitchScreenshot->sizePolicy().hasHeightForWidth());
        pbSwitchScreenshot->setSizePolicy(sizePolicy1);
        pbSwitchScreenshot->setMinimumSize(QSize(20, 20));
        pbSwitchScreenshot->setMaximumSize(QSize(20, 20));
        pbSwitchScreenshot->setCheckable(true);
        pbSwitchScreenshot->setFlat(true);

        horizontalLayout->addWidget(pbSwitchScreenshot);

        horizontalSpacer = new QSpacerItem(599, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbCheckLowVol = new QPushButton(titleFrame);
        pbCheckLowVol->setObjectName(QStringLiteral("pbCheckLowVol"));
        sizePolicy1.setHeightForWidth(pbCheckLowVol->sizePolicy().hasHeightForWidth());
        pbCheckLowVol->setSizePolicy(sizePolicy1);
        pbCheckLowVol->setMinimumSize(QSize(20, 20));
        pbCheckLowVol->setMaximumSize(QSize(20, 20));
        pbCheckLowVol->setCheckable(true);
        pbCheckLowVol->setChecked(true);
        pbCheckLowVol->setFlat(true);

        horizontalLayout->addWidget(pbCheckLowVol);

        horizontalSpacer_10 = new QSpacerItem(32, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_10);

        pbCheckInfo = new QPushButton(titleFrame);
        pbCheckInfo->setObjectName(QStringLiteral("pbCheckInfo"));
        sizePolicy1.setHeightForWidth(pbCheckInfo->sizePolicy().hasHeightForWidth());
        pbCheckInfo->setSizePolicy(sizePolicy1);
        pbCheckInfo->setMinimumSize(QSize(20, 20));
        pbCheckInfo->setMaximumSize(QSize(20, 20));
        pbCheckInfo->setCheckable(true);
        pbCheckInfo->setChecked(true);
        pbCheckInfo->setFlat(true);

        horizontalLayout->addWidget(pbCheckInfo);

        pbCheckTimer = new QPushButton(titleFrame);
        pbCheckTimer->setObjectName(QStringLiteral("pbCheckTimer"));
        sizePolicy1.setHeightForWidth(pbCheckTimer->sizePolicy().hasHeightForWidth());
        pbCheckTimer->setSizePolicy(sizePolicy1);
        pbCheckTimer->setMinimumSize(QSize(20, 20));
        pbCheckTimer->setMaximumSize(QSize(20, 20));
        pbCheckTimer->setCheckable(true);
        pbCheckTimer->setChecked(true);
        pbCheckTimer->setFlat(true);

        horizontalLayout->addWidget(pbCheckTimer);

        horizontalSpacer_11 = new QSpacerItem(16, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_11);

        pbCheckWeapon = new QPushButton(titleFrame);
        pbCheckWeapon->setObjectName(QStringLiteral("pbCheckWeapon"));
        sizePolicy1.setHeightForWidth(pbCheckWeapon->sizePolicy().hasHeightForWidth());
        pbCheckWeapon->setSizePolicy(sizePolicy1);
        pbCheckWeapon->setMinimumSize(QSize(20, 20));
        pbCheckWeapon->setMaximumSize(QSize(20, 20));
        pbCheckWeapon->setCheckable(true);
        pbCheckWeapon->setChecked(false);
        pbCheckWeapon->setFlat(true);

        horizontalLayout->addWidget(pbCheckWeapon);

        horizontalSpacer_5 = new QSpacerItem(16, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        comboBoxZoom = new QComboBox(titleFrame);
        comboBoxZoom->setObjectName(QStringLiteral("comboBoxZoom"));
        sizePolicy1.setHeightForWidth(comboBoxZoom->sizePolicy().hasHeightForWidth());
        comboBoxZoom->setSizePolicy(sizePolicy1);
        comboBoxZoom->setMinimumSize(QSize(60, 20));
        comboBoxZoom->setMaximumSize(QSize(60, 20));

        horizontalLayout->addWidget(comboBoxZoom);

        horizontalSpacer_4 = new QSpacerItem(8, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        pbMoveTogether = new QPushButton(titleFrame);
        pbMoveTogether->setObjectName(QStringLiteral("pbMoveTogether"));
        sizePolicy1.setHeightForWidth(pbMoveTogether->sizePolicy().hasHeightForWidth());
        pbMoveTogether->setSizePolicy(sizePolicy1);
        pbMoveTogether->setMinimumSize(QSize(20, 20));
        pbMoveTogether->setMaximumSize(QSize(20, 20));
        pbMoveTogether->setCheckable(true);
        pbMoveTogether->setChecked(true);
        pbMoveTogether->setFlat(true);

        horizontalLayout->addWidget(pbMoveTogether);

        horizontalSpacer_7 = new QSpacerItem(16, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);

        pbCheckTrasparent = new QPushButton(titleFrame);
        pbCheckTrasparent->setObjectName(QStringLiteral("pbCheckTrasparent"));
        sizePolicy1.setHeightForWidth(pbCheckTrasparent->sizePolicy().hasHeightForWidth());
        pbCheckTrasparent->setSizePolicy(sizePolicy1);
        pbCheckTrasparent->setMinimumSize(QSize(20, 20));
        pbCheckTrasparent->setMaximumSize(QSize(20, 20));
        pbCheckTrasparent->setCheckable(true);
        pbCheckTrasparent->setFlat(true);

        horizontalLayout->addWidget(pbCheckTrasparent);

        horizontalSpacer_6 = new QSpacerItem(16, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        pbMinimize = new QPushButton(titleFrame);
        pbMinimize->setObjectName(QStringLiteral("pbMinimize"));
        sizePolicy1.setHeightForWidth(pbMinimize->sizePolicy().hasHeightForWidth());
        pbMinimize->setSizePolicy(sizePolicy1);
        pbMinimize->setMinimumSize(QSize(20, 20));
        pbMinimize->setMaximumSize(QSize(20, 20));
        pbMinimize->setFlat(true);

        horizontalLayout->addWidget(pbMinimize);

        pbClose = new QPushButton(titleFrame);
        pbClose->setObjectName(QStringLiteral("pbClose"));
        sizePolicy1.setHeightForWidth(pbClose->sizePolicy().hasHeightForWidth());
        pbClose->setSizePolicy(sizePolicy1);
        pbClose->setMinimumSize(QSize(20, 20));
        pbClose->setMaximumSize(QSize(20, 20));
        pbClose->setFlat(true);

        horizontalLayout->addWidget(pbClose);


        verticalLayout->addWidget(titleFrame);

        webFrame = new QFrame(centralWidget);
        webFrame->setObjectName(QStringLiteral("webFrame"));
        webFrame->setFrameShape(QFrame::NoFrame);
        webFrame->setFrameShadow(QFrame::Plain);
        webFrame->setLineWidth(0);
        verticalLayout_2 = new QVBoxLayout(webFrame);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        webView = new QWebView(webFrame);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));

        verticalLayout_2->addWidget(webView);


        verticalLayout->addWidget(webFrame);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pbRefresh->setText(QApplication::translate("MainWindow", "\346\226\260", 0));
        pbScreenshot->setText(QApplication::translate("MainWindow", "\346\222\256", 0));
        pbSwitchScreenshot->setText(QApplication::translate("MainWindow", "\351\200\243", 0));
        pbCheckLowVol->setText(QApplication::translate("MainWindow", "\351\235\231", 0));
        pbCheckInfo->setText(QApplication::translate("MainWindow", "\346\203\205", 0));
        pbCheckTimer->setText(QApplication::translate("MainWindow", "\346\231\202", 0));
        pbCheckWeapon->setText(QApplication::translate("MainWindow", "\346\255\246", 0));
        comboBoxZoom->clear();
        comboBoxZoom->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\255\211\345\200\215", 0)
         << QApplication::translate("MainWindow", "50%", 0)
         << QApplication::translate("MainWindow", "75%", 0)
         << QApplication::translate("MainWindow", "100%", 0)
         << QApplication::translate("MainWindow", "125%", 0)
         << QApplication::translate("MainWindow", "150%", 0)
         << QApplication::translate("MainWindow", "175%", 0)
         << QApplication::translate("MainWindow", "200%", 0)
        );
        pbMoveTogether->setText(QApplication::translate("MainWindow", "\347\247\273", 0));
        pbCheckTrasparent->setText(QApplication::translate("MainWindow", "\351\200\217", 0));
        pbMinimize->setText(QApplication::translate("MainWindow", "-", 0));
        pbClose->setText(QApplication::translate("MainWindow", "X", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

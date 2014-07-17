﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>

#include <Windows.h>

MainWindow * MainWindow::s_pMainWindow = 0;

MainWindow::MainWindow(QWidget *parent) :
    MainWindowBase(parent),
    ui(new Ui::MainWindow)
{
    setMainWindow(this);
    ui->setupUi(this);
    ui->retranslateUi(this);

    ui->comboBoxZoom->insertSeparator(1);

    mwbPostInit();

    ui->titleFrame->setHandlingWidget(this);

    this->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::postInit(InfoMainWindow *pInfo, TimerMainWindow *pTimer)
{
    m_pInfoWindow = pInfo;
    m_pTimerWindow = pTimer;

    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );

    connect(m_pInfoWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
    connect(m_pTimerWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));


    connect(ui->pbCheckInfo, SIGNAL(toggled(bool)), m_pInfoWindow, SLOT(setVisible(bool)));
    connect(ui->pbCheckTimer, SIGNAL(toggled(bool)), m_pTimerWindow, SLOT(setVisible(bool)));
}

void MainWindow::onSubMainWindowShowHide(bool bShow, MainWindowBase *pWindow)
{
    QPushButton * pButton = 0;
    if (pWindow == m_pInfoWindow)
    {
        pButton = ui->pbCheckInfo;
    }
    else if (pWindow == m_pTimerWindow)
    {
        pButton = ui->pbCheckTimer;
    }
    if (!pButton)
    {
        return;
    }

    pButton->setChecked(bShow);
}


void MainWindow::changeEvent(QEvent *e)
{
    MainWindowBase::changeEvent(e);

    if (e->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);
        if(event->oldState() & Qt::WindowMinimized)
        {
            m_pInfoWindow->setWindowState(Qt::WindowNoState);
            m_pTimerWindow->setWindowState(Qt::WindowNoState);

            m_pInfoWindow->show();
            m_pTimerWindow->show();
            m_pInfoWindow->stackUnder(this);
            m_pTimerWindow->stackUnder(m_pInfoWindow);
            this->raise();
            this->activateWindow();
        }
        else if (isMinimized())
        {
            if (!m_pInfoWindow->isDockingOnTop())
            {
                m_pInfoWindow->hide();
            }
            if (!m_pTimerWindow->isDockingOnTop())
            {
                m_pTimerWindow->hide();
            }
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *e)
{
//    m_pFiddler->Shutdown();
    MainWindowBase::closeEvent(e);

    m_pInfoWindow->close();
    m_pTimerWindow->close();
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    MainWindowBase::moveEvent(e);

    QPoint diffPos = e->pos()-e->oldPos();

    if (!m_pInfoWindow->isDockingOnTop())
    {
        m_pInfoWindow->move(m_pInfoWindow->pos()+diffPos);
    }
    if (!m_pTimerWindow->isDockingOnTop())
    {
        m_pTimerWindow->move(m_pTimerWindow->pos()+diffPos);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QRect rc = QApplication::desktop()->availableGeometry(QApplication::desktop()->primaryScreen());
    if (x() < 20 && x() > -20 && x()!=0)
    {
        move(0, y());
    }
    else if (x()+width() > rc.width()-20 && x()+width() < rc.width()+20 && x()!=rc.width()-width())
    {
        move(rc.width()-width(), y());
    }
    if (y() < 20 && y()!=0)
    {
        move(x(), 0);
    }
    MainWindowBase::mouseReleaseEvent(event);
}

void MainWindow::slotActivate(QWidget *w, bool bActivate)
{
    this->raise();
    w->stackUnder(this);
}

void MainWindow::slotToggleRestoreMinimize(bool bRestore)
{
    if (bRestore)
    {
        if (isMinimized())
        {
            this->setWindowState(Qt::WindowNoState);
        }
    }
    if (!bRestore)
    {
        if (!isMinimized())
        {
            this->setWindowState(Qt::WindowMinimized);
        }
    }
}

void MainWindow::on_pbClose_clicked()
{
    close();
}

void MainWindow::on_pbMinimize_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

void MainWindow::on_pbCheckTrasparent_toggled(bool checked)
{
    static QList<QPair<QString, QWidget*>> s_listpair;
    if (checked)
    {
        s_listpair.clear();

        QList<QWidget *> lstWidget = findChildren<QWidget *>();
        QWidget * w;
        foreach(w, lstWidget)
        {
            QPair<QString, QWidget*> pair;
            pair.first = w->styleSheet();
            pair.second = w;
            s_listpair.append(pair);
        }

        lstWidget = m_pInfoWindow->findChildren<QWidget *>();
        foreach(w, lstWidget)
        {
            QPair<QString, QWidget*> pair;
            pair.first = w->styleSheet();
            pair.second = w;
            s_listpair.append(pair);
        }
        lstWidget = m_pTimerWindow->findChildren<QWidget *>();
        foreach(w, lstWidget)
        {
            QPair<QString, QWidget*> pair;
            pair.first = w->styleSheet();
            pair.second = w;
            s_listpair.append(pair);
        }

        QPair<QString, QWidget*> p;
        foreach(p, s_listpair)
        {
            w = p.second;
            QString stylestr = w->styleSheet();
            stylestr += "background-color: rgba(45, 45, 48, 30%);";
            w->setStyleSheet(stylestr);
        }

    }
    else
    {
        QPair<QString, QWidget*> p;
        foreach(p, s_listpair)
        {
            if (isAncestorOf(p.second) || m_pInfoWindow->isAncestorOf(p.second) || m_pTimerWindow->isAncestorOf(p.second))
            {
                p.second->setStyleSheet(p.first);
            }
        }
        s_listpair.clear();
    }

}

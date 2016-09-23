//
//  MainWindow_Mac.m
//  KanPlay
//
//
//

#include "mainwindowbase.h"
#include <QApplication>

#import <cocoa/cocoa.h>

void MainWindowBase::minimizeWindow()
{
    /*
    for (auto widget : QApplication::topLevelWidgets())
    {
        NSView *view = (NSView *)widget->winId();
        NSWindow *wnd = [view window];
        [wnd miniaturize:wnd];
        
    }
     */
    NSView *view = (NSView *)window()->winId();
    NSWindow *wnd = [view window];
    [wnd miniaturize:wnd];
}

void MainWindowBase::restoreWindow()
{
    /*
    for (auto widget : QApplication::topLevelWidgets())
    {
        NSView *view = (NSView *)widget->winId();
        NSWindow *wnd = [view window];
        [wnd deminiaturize:wnd];
    }
    */
    NSView *view = (NSView *)window()->winId();
    NSWindow *wnd = [view window];
    [wnd deminiaturize:wnd];
}

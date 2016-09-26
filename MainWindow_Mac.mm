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

void MainWindowBase::clickAtGlobalPos(const QPoint& pt)
{
    CGEventRef click_down = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown,
                                                    CGPointMake(pt.x(), pt.y()),
                                                    kCGMouseButtonLeft
                                                    );
    
    CGEventRef click_up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp,
                                                  CGPointMake(pt.x(), pt.y()),
                                                  kCGMouseButtonLeft
                                                  );
    
    
    CGEventPost(kCGHIDEventTap, click_down);
    CGEventPost(kCGHIDEventTap, click_up);
    
    CFRelease(click_down);
    CFRelease(click_up);
    
}
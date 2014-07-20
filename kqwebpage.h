#ifndef KQWEBPAGE_H
#define KQWEBPAGE_H

#include <QWebPage>
#include <QApplication>

class KQWebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit KQWebPage(QWidget *parent = 0);

signals:

public slots:
    virtual bool shouldInterruptJavaScript() {
//        QApplication::processEvents(QEventLoop::AllEvents, 42);
        // Ignore the error
        return false;
    }

};

#endif // KQWEBPAGE_H

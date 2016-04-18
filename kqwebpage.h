#ifndef KQWebPage_H
#define KQWebPage_H

#include <QWebEnginePage>
#include <QApplication>

class KQWebPage : public QWebEnginePage
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

#endif // KQWebPage_H

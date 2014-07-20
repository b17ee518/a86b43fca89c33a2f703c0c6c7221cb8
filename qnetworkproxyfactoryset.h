#ifndef QNETWORKPROXYFACTORYSET_H
#define QNETWORKPROXYFACTORYSET_H

#include <QNetworkProxyFactory>

class QNetworkProxyFactorySet : public QNetworkProxyFactory
{
public:
    QNetworkProxyFactorySet():QNetworkProxyFactory(){};
    virtual QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query);

    void init(int useport);

private:
    QNetworkProxy m_proxyhttp;
    QNetworkProxy m_proxyhttps;
};

#endif // QNETWORKPROXYFACTORYSET_H

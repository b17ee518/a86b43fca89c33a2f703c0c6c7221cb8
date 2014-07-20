#include "qnetworkproxyfactoryset.h"

QList<QNetworkProxy> QNetworkProxyFactorySet::queryProxy(const QNetworkProxyQuery &query)
{
    QList<QNetworkProxy> lst;

    QString str = query.url().toString();
    if (str.startsWith("http://"))
    {
        return lst << m_proxyhttp;
    }

    if (str.startsWith("https://"))
    {
        return lst << m_proxyhttps;
    }

    return lst<<QNetworkProxy::NoProxy;
}

void QNetworkProxyFactorySet::init(int useport)
{
    m_proxyhttp.setType(QNetworkProxy::HttpProxy);
    m_proxyhttp.setHostName("127.0.0.1");
    m_proxyhttp.setPort(useport);

    m_proxyhttps.setType(QNetworkProxy::HttpProxy);
    m_proxyhttps.setHostName("127.0.0.1");
    m_proxyhttps.setPort(useport);
}

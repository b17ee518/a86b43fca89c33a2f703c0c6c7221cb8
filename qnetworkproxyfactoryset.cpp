#include "qnetworkproxyfactoryset.h"

QList<QNetworkProxy> QNetworkProxyFactorySet::queryProxy(const QNetworkProxyQuery &query)
{
	QList<QNetworkProxy> lst;

	QString str = query.url().toString();
	if (str.startsWith("http://"))
	{
		return lst << _proxyhttp;
	}

	if (str.startsWith("https://") || str.endsWith(":443"))
	{
		return lst << _proxyhttps;
	}

	return lst << QNetworkProxy::NoProxy;
}

void QNetworkProxyFactorySet::init(int useport)
{
	_proxyhttp.setType(QNetworkProxy::HttpProxy);
	_proxyhttp.setHostName("127.0.0.1");
	_proxyhttp.setPort(useport);

	_proxyhttps.setType(QNetworkProxy::HttpProxy);
	_proxyhttps.setHostName("127.0.0.1");
	_proxyhttps.setPort(useport);
}

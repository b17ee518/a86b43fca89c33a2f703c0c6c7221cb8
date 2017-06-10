#ifndef QNETWORKPROXYFACTORYSET_H
#define QNETWORKPROXYFACTORYSET_H

#include <QNetworkProxyFactory>

class QNetworkProxyFactorySet : public QNetworkProxyFactory
{
public:
	QNetworkProxyFactorySet() :QNetworkProxyFactory(){};
	virtual QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query);

	void init(int useport);

	const QNetworkProxy& getHttpProxy() { return _proxyhttp; }
	const QNetworkProxy& getHttpsProxy() { return _proxyhttps; }

private:
	QNetworkProxy _proxyhttp;
	QNetworkProxy _proxyhttps;
};

#endif // QNETWORKPROXYFACTORYSET_H

#ifndef KQNETWORKACCESSMANAGER_H
#define KQNETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class KQNetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT
public:
	explicit KQNetworkAccessManager(QObject *parent = 0);

protected:
	virtual QNetworkReply * createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData);

signals:

	public slots :
		void slotReadyRead();
};

#endif // KNETWORKACCESSMANAGER_H

#ifndef KQNETWORKREPLY_H
#define KQNETWORKREPLY_H

#include <QNetworkReply>

class KQNetworkReply : public QNetworkReply
{
	Q_OBJECT

public:
	KQNetworkReply(QObject *parent, QNetworkReply *toCopy, QNetworkAccessManager *mgr);
	~KQNetworkReply();

	void copyAttribute(QNetworkRequest::Attribute attr);

	qint64 bytesAvailable() const;

	void ignoreSslErrors(const QList<QSslError> &errors);
	QSslConfiguration sslConfiguration() const;
	void setSslConfiguration(const QSslConfiguration &config);
	QNetworkAccessManager* manager() const;
	bool event(QEvent *e);

	void abort();
	void ignoreSslErrors();

	bool isSequential() const;

	protected slots:
	void handleResponse();

protected:
	qint64 readData(char *data, qint64 maxSize);

private:
	struct KQNetworkReplyPrivate *d = NULL;
};

#endif // KQNETWORKREPLY_H

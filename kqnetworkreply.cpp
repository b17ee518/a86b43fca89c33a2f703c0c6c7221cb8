#include "KQNetworkReply.h"
#include "mainwindow.h"

#include <QStandardPaths>
#include <QSslConfiguration>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QBuffer>
#include <QTimer>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include "RemoteNotifyHandler.h"

struct KQNetworkReplyPrivate{
	QNetworkReply *copied;

	QByteArray content;
	qint64 offset;
	bool finished;

	QNetworkAccessManager *manager;
};

KQNetworkReply::KQNetworkReply(QObject *parent, QNetworkReply *toCopy, QNetworkAccessManager *mgr)
	: QNetworkReply(parent) {
	d = new KQNetworkReplyPrivate;
	d->finished = false;
	d->copied = toCopy;
	d->manager = mgr;

	setOperation(d->copied->operation());
	setRequest(d->copied->request());
	setUrl(d->copied->url());

	// Translate reply when it's complete
	connect(d->copied, SIGNAL(finished()), SLOT(handleResponse()));

	connect(d->copied, SIGNAL(encrypted()), SIGNAL(encrypted()));
	connect(d->copied, SIGNAL(metaDataChanged()), SIGNAL(metaDataChanged()));
}


KQNetworkReply::~KQNetworkReply()
{
	if (d)
	{
		delete d;
	}
}

void KQNetworkReply::copyAttribute(QNetworkRequest::Attribute attr) {
	QVariant attribute = d->copied->attribute(attr);
	if (attribute.isValid())
		setAttribute(attr, attribute);
}

void KQNetworkReply::handleResponse() {
	if (d->finished) return;

	setError(d->copied->error(), d->copied->errorString());

	copyAttribute(QNetworkRequest::HttpStatusCodeAttribute);
	copyAttribute(QNetworkRequest::HttpReasonPhraseAttribute);
	copyAttribute(QNetworkRequest::RedirectionTargetAttribute);
	copyAttribute(QNetworkRequest::ConnectionEncryptedAttribute);
	copyAttribute(QNetworkRequest::SourceIsFromCacheAttribute);
	copyAttribute(QNetworkRequest::HttpPipeliningWasUsedAttribute);

	QList<QNetworkReply::RawHeaderPair> headers = d->copied->rawHeaderPairs();
	for (int i = 0; i < headers.size(); i++)
		setRawHeader(headers[i].first, headers[i].second);

	QByteArray data = d->copied->readAll();
	d->copied->abort();

	//	qDebug() << "content:" << data;

	d->content = data;
	d->offset = 0;
	setHeader(QNetworkRequest::ContentLengthHeader, QVariant(d->content.size()));

	open(ReadOnly | Unbuffered);

	QString mimetype = d->copied->header(QNetworkRequest::ContentTypeHeader).toString();
	if (mimetype.contains("text/plain"))
	{
		QUrl url = d->copied->request().url();
		QString PathAndQuery = url.path();
		if (PathAndQuery.startsWith("/kcsapi"))
		{
			QString responseBody = QString::fromUtf8(d->content.constData(), d->content.size());
			QString requestBody = property("requestBody").toString();

			QVariant statusCode = d->copied->attribute(QNetworkRequest::HttpStatusCodeAttribute);
			if (!statusCode.isValid() || statusCode.toInt() != 200)
			{
				RemoteNotifyHandler::getInstance().NotifyNeko();
			}

			MainWindow::mainWindow()->onGetNetworkReply(PathAndQuery, requestBody, responseBody);
		}
	}

	d->finished = true;

	emit finished();
	emit readChannelFinished();
}

qint64 KQNetworkReply::bytesAvailable() const {
	return d->content.size() - d->offset + QIODevice::bytesAvailable();
}

qint64 KQNetworkReply::readData(char *data, qint64 maxSize) {
	if (d->offset >= d->content.size())
		return -1;

	qint64 numBytes = qMin(maxSize, d->content.size() - d->offset);
	memcpy(data, d->content.constData() + d->offset, numBytes);
	d->offset += numBytes;

	return numBytes;
}

void KQNetworkReply::ignoreSslErrors(const QList<QSslError> &errors) {
	d->copied->ignoreSslErrors(errors);
}

QNetworkAccessManager* KQNetworkReply::manager() const {
	return d->copied->manager();
}

void KQNetworkReply::setSslConfiguration(const QSslConfiguration &config) {
	d->copied->setSslConfiguration(config);
}

QSslConfiguration KQNetworkReply::sslConfiguration() const {
	return d->copied->sslConfiguration();
}

bool KQNetworkReply::event(QEvent *e) {
	return d->copied->event(e);
}

void KQNetworkReply::abort() {
	d->finished = true; d->copied->abort();
}

void KQNetworkReply::ignoreSslErrors() {
	d->copied->ignoreSslErrors();
}

bool KQNetworkReply::isSequential() const {
	return true;
}

#include "kqnetworkaccessmanager.h"
#include <QFile>
#include "mainwindow.h"
#include "kqnetworkreply.h"

#define REQBODY_BUFFER_SIZE    1024*16

KQNetworkAccessManager::KQNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
}

QNetworkReply *KQNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &req, QIODevice *outgoingData)
{
	QNetworkRequest request = req;
    
    
	if (op == PostOperation) {
		//qDebug() << "POST" << request.url().path();
		// If the request addressed to API - translate this request
		if (request.url().host() != "localhost" && request.url().host() != "127.0.0.1" && !request.url().host().contains(".dmm.com")) {
			QNetworkReply *r = QNetworkAccessManager::createRequest(op, request, outgoingData);
            
            KQNetworkReply *reply = new KQNetworkReply(r->parent(), r, this);
            if (outgoingData)
            {
                QString requestBody = outgoingData->peek(REQBODY_BUFFER_SIZE);
                reply->setProperty("requestBody", requestBody);
            }
			return reply;
		}
	}
	/*
	if (outgoingData)
	{
		QString requestBody = outgoingData->peek(outgoingData->size() + 1);
		reply->setProperty("requestBody", requestBody);
	}
//	reply->setUserData()
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	*/
	QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    return reply;
}

void KQNetworkAccessManager::slotReadyRead()
{
    QNetworkReply * reply = static_cast<QNetworkReply*>(sender());

	if (reply->request().url().path().startsWith("/kcsapi"))
	{
		QByteArray data = reply->peek(reply->bytesAvailable());
		qDebug(QString::number(data.size()).toLocal8Bit());
	}
	MainWindow::mainWindow()->onGetNetworkReply(reply);
/*

    QString mimetype = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    if (mimetype.contains("text/plain"))
    {
        QByteArray replybytes = reply->peek(reply->bytesAvailable()+1);

		/ *
        QFile file("C:/data.txt");
        file.open(QIODevice::Append);
        file.write(reply->request().url().toString().toLocal8Bit().constData());
        file.write("\r\n\r\n");
        file.write(replybytes);
        file.write("\r\n\r\n");
        file.close();
		* /
    }*/
	
}

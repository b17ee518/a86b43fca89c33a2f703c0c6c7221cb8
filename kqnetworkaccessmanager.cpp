#include "kqnetworkaccessmanager.h"
#include <QFile>
#include "mainwindow.h"

KQNetworkAccessManager::KQNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
	qDebug("");
}

QNetworkReply *KQNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
	
	if (outgoingData)
	{
		qDebug(outgoingData->peek(outgoingData->size() + 1));
	}
//	reply->setUserData()
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    return reply;
}

void KQNetworkAccessManager::slotReadyRead()
{
    QNetworkReply * reply = static_cast<QNetworkReply*>(sender());

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

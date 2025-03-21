#include "ApiClient.h"
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

ApiClient::ApiClient(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ApiClient::onReplyReceived);
}

void ApiClient::makeRequest(const QUrl &url) {
    emit statusUpdated("Loading...");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
    qDebug() << "Request at: " << url;
}

void ApiClient::onReplyReceived(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << "Received JSON";
            emit statusUpdated("Success");
            emit dataRecived(jsonObj);
        }
    } else {
        emit statusUpdated("Error");
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}

#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);
    void makeRequest(const QUrl &url);

private slots:
    void onReplyReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;

signals:
    void statusUpdated(const QString &status);
    void dataRecived(const QJsonObject &data);
};

#endif // APICLIENT_H

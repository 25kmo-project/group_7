#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

class ApiClient : public QObject
{
    Q_OBJECT

public:
    static ApiClient& instance();

    void setToken(const QByteArray& t);
    QByteArray getToken() const;

    QNetworkReply* get(const QString& endpoint);
    QNetworkReply* post(const QString& endpoint, const QJsonObject& body);

private:
    explicit ApiClient(QObject *parent = nullptr);

    QByteArray token;
    QNetworkAccessManager manager;
};

#endif // APICLIENT_H

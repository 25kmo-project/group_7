/**
 * @file apiclient.cpp
 * @brief Singleton API-client kaikkien backend kutsujen hoitamiseen
 */

#include "apiclient.h"
#include "environment.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

ApiClient& ApiClient::instance()
{
    static ApiClient inst;
    return inst;
}
/**
 * @brief Tekee POST-pyynnön
 * @param endpoint
 * @param json Lähetettävä JSON-data.
 * @return QNetWorkReply*
 */
ApiClient::ApiClient(QObject *parent)
    : QObject(parent)
{
}

void ApiClient::setToken(const QByteArray& t)
{
    token = t;
}

QByteArray ApiClient::getToken() const
{
    return token;
}

QNetworkReply* ApiClient::get(const QString& endpoint)
{
    QUrl url(Environment::base_url() + endpoint);
    QNetworkRequest req(url);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer " + token);

    return manager.get(req);
}

QNetworkReply* ApiClient::post(const QString& endpoint, const QJsonObject& body)
{
    QUrl url(Environment::base_url() + endpoint);
    QNetworkRequest req(url);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer " + token);

    QJsonDocument doc(body);
    return manager.post(req, doc.toJson());
}

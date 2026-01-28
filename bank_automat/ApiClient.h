#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

/**
 * @class ApiClient
 * @brief Yksittäinen luokka backend-API-kutsujen tekemiseen.
 *
 * ApiClient toimii sovelluksen keskitettynä HTTP-rajapintana.
 * Luokka toteutetaan singleton-mallilla, jotta kaikki sovelluksen
 * osat käyttävät samaa QNetworkAccessManager-instanssia ja samaa
 * JWT-tokenia. Tämä helpottaa autentikointia ja vähentää
 * päällekkäisiä yhteyksiä.
 */
class ApiClient : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Palauttaa ApiClientin singleton-instanssin.
     *
     * Tätä kautta kaikki luokat käyttävät samaa API-yhteyttä.
     */
    static ApiClient& instance();

    /**
     * @brief Asettaa JWT-tokenin, joka lisätään kaikkiin API-kutsuihin.
     * @param t Token, jonka backend palauttaa kirjautumisen yhteydessä.
     */
    void setToken(const QByteArray& t);

    /**
     * @brief Palauttaa tällä hetkellä käytössä olevan JWT-tokenin.
     * @return Token raakamuodossa.
     */
    QByteArray getToken() const;

    /**
     * @brief Suorittaa GET-pyynnön annetulle endpointille.
     * @param endpoint Backendin polku, esim. "bank_account/1/debit".
     * @return QNetworkReply-olio, jonka kautta vastaus luetaan.
     *
     * Token lisätään automaattisesti Authorization-headeriin.
     */
    QNetworkReply* get(const QString& endpoint);

    /**
     * @brief Suorittaa POST-pyynnön annetulle endpointille.
     * @param endpoint Backendin polku.
     * @param body JSON-data, joka lähetetään POST-pyynnön mukana.
     * @return QNetworkReply-olio, jonka kautta vastaus luetaan.
     *
     * Token lisätään automaattisesti Authorization-headeriin.
     */
    QNetworkReply* post(const QString& endpoint, const QJsonObject& body);

private:
    /**
     * @brief Yksityinen konstruktori singleton-mallia varten.
     */
    explicit ApiClient(QObject *parent = nullptr);

    QByteArray token;              ///< JWT-token, lisätään kaikkiin API-kutsuihin
    QNetworkAccessManager manager; ///< Qt:n HTTP-yhteyksien hallinta
};

#endif // APICLIENT_H

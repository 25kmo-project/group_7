#ifndef DATA_H
#define DATA_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDialog>

namespace Ui {
class Data;
}

/**
 * @class Data
 * @brief Dialogi, joka näyttää käyttäjän perustiedot JSON-datasta.
 *
 * Tämä luokka vastaanottaa backendiltä saadun JSON-olion ja
 * näyttää sen sisällön käyttöliittymässä (etunimi, sukunimi, osoite).
 * Lisäksi dialogista voi palata takaisin tai kirjautua ulos.
 */
class Data : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Luo Data-dialogin.
     * @param parent Vanhempi widget, yleensä Accountinfo-ikkuna.
     */
    explicit Data(QWidget *parent = nullptr);

    /**
     * @brief Data-luokan destruktori.
     */
    ~Data();

    /**
     * @brief Asettaa JSON-datan ja päivittää UI-elementit.
     * @param newTestData JSON-data QByteArray-muodossa.
     *
     * Funktio parsii JSON-datan ja asettaa käyttöliittymän kentät.
     * Jos data on virheellistä tai avaimia puuttuu, käytetään
     * fallback-arvoa "Ei dataa".
     */
    void setTestData(const QByteArray &newTestData);

signals:
    /**
     * @brief Lähetetään, kun käyttäjä painaa "Kirjaudu ulos" -nappia.
     *
     * Accountinfo-ikkuna kuuntelee tätä signaalia ja sulkee itsensä.
     */
    void logoutRequested();

private slots:
    /**
     * @brief Käsittelee Takaisin-napin painalluksen.
     *
     * Sulkee Data-ikkunan ja palaa edelliseen näkymään.
     */
    void btnBackClicked();

    /**
     * @brief Käsittelee Kirjaudu ulos -napin painalluksen.
     *
     * Sulkee Data-ikkunan, lähettää logoutRequested-signaalin
     * ja avaa uuden MainWindow-instanssin kirjautumista varten.
     */
    void btnLogOutClicked();

private:
    Ui::Data *ui;          ///< Käyttöliittymäolio (Qt Designerin generoima)
    QByteArray testData;   ///< Tallennettu JSON-data raakamuodossa
};

#endif // DATA_H

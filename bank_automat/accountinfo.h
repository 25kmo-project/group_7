#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class Accountinfo;
}

/**
 * @class Accountinfo
 * @brief Näyttää käyttäjän tilitiedot ja tarjoaa toiminnot (nosto, talletus, siirto).
 *
 * Tämä dialogi avautuu kirjautumisen jälkeen ja näyttää käyttäjän tilin saldon,
 * tilinumeron ja muut perustiedot. Ikkunasta voi siirtyä henkilötietoihin,
 * tehdä nostoja, talletuksia ja tilisiirtoja. Lisäksi käyttäjä voi palata
 * takaisin tai kirjautua ulos.
 */
class Accountinfo : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Luo Accountinfo-dialogin.
     * @param parent Vanhempi widget, yleensä MainWindow.
     */
    explicit Accountinfo(QWidget *parent = nullptr);

    /**
     * @brief Accountinfo-luokan destruktori.
     */
    ~Accountinfo();

    /**
     * @brief Asettaa käyttäjän ID:n tekstimuodossa.
     * @param newUsername Käyttäjän ID merkkijonona.
     */
    void setUsername(const QString &newUsername);

    /**
     * @brief Asettaa JWT-tokenin UI-käyttöä varten.
     * @param newToken Token, jota käytetään API-kutsuissa.
     *
     * Huom: Varsinaiset API-kutsut tekee ApiClient, mutta token
     * voidaan näyttää UI:ssa tai välittää aliluokille.
     */
    void setToken(const QByteArray &newToken);

    /**
     * @brief Asettaa tilin JSON-datan ja täyttää käyttöliittymän kentät.
     * @param obj JSON-olio, joka sisältää tilin tiedot (saldo, tilinumero, jne.).
     */
    void setAccountData(const QJsonObject &obj);

    /**
     * @brief Asettaa tilin tyypin (debit/credit).
     * @param type Tilin tyyppi merkkijonona.
     */
    void setAccountType(const QString &type);

    /**
     * @brief Asettaa tilin ID:n.
     * @param id Tilin yksilöllinen tunniste backendissä.
     */
    void setAccountId(int id);

protected:
    /**
     * @brief Suoritetaan aina, kun ikkuna näytetään.
     * @param event Qt:n showEvent-tapahtuma.
     *
     * Tätä käytetään esimerkiksi saldon päivittämiseen aina,
     * kun käyttäjä palaa tähän näkymään.
     */
    void showEvent(QShowEvent *event) override;

signals:
    /**
     * @brief Lähetetään, kun käyttäjä haluaa palata takaisin.
     *
     * MainWindow kuuntelee tätä signaalia ja näyttää itsensä uudelleen.
     */
    void backRequested();

private slots:
    /**
     * @brief Avaa henkilötietonäkymän (Data-dialogin).
     */
    void btnMyDataClicked();

    /**
     * @brief Avaa nostonäkymän.
     */
    void btnWithdrawClicked();

    /**
     * @brief Avaa talletusnäkymän.
     */
    void btnNewDepositClicked();

    /**
     * @brief Avaa tilisiirtonäkymän.
     */
    void btnTransferClicked();

    /**
     * @brief Päivittää saldon noston tai talletuksen jälkeen.
     */
    void refreshBalance();

    /**
     * @brief Käsittelee Takaisin-napin painalluksen.
     */
    void on_btnBack_clicked();

    /**
     * @brief Käsittelee Kirjaudu ulos -napin painalluksen.
     */
    void on_btnLogout_clicked();

private:
    Ui::Accountinfo *ui;   ///< Qt Designerin generoima käyttöliittymäolio

    QString username;      ///< Käyttäjän ID merkkijonona
    QByteArray token;      ///< JWT-token (vain UI:lle, ApiClient hoitaa kutsut)
    QString accountType;   ///< Tilin tyyppi (debit/credit)
    int accountId = -1;    ///< Tilin yksilöllinen tunniste
};

#endif // ACCOUNTINFO_H

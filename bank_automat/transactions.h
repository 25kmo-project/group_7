#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class Transactions;
}
/**
 * @class Transactions
 * @brief Näyttää käyttäjän tilitapahtumat.
 *
 * Tämä dialogi hakee ja näyttää tilitapahtumat taulukossa. Tapahtumien paginointi
 * hoidetaan backendissä (10 tapahtumaa kerrallaan). JSON-muotoinen data
 * näytetään kolmessa sarakkeessa: päivämäärä, tapahtuma ja summa.
 * Sovellus muuntaa UTC-aikavyöhykkeellä olevat aikaleimat paikalliseen aikaan.
 */
class Transactions : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Luo Transactions-dialogin.
     * @param parent Vanhempi QWidget, yleensä nullptr.
     */
    explicit Transactions(QWidget *parent = nullptr);

    /**
     * @brief Tuhoaa Transactions-olion ja vapauttaa käyttöliittymän.
     */
    ~Transactions();

    /**
     * @brief Asettaa backend-tokenin autentikointia varten.
     * @param t Token merkkijonona.
     */
    void setToken(const QString &t);

    /**
     * @brief Asettaa tilin ID:n, jonka tapahtumia tarkastellaan
     * @param id Tilin tunniste.
     */
    void setAccountId(int id);

    /**
     * @brief Haetaan tilitapahtumat backendistä ja päivittää taulukkonäkymän.
     */
    void updateLogs();

private slots:
    /**
     * @brief Sulkee dialogin ja palaa takaisin edelliseen näkymään.
     */
    void btnCloseClicked();

    /**
     * @brief Siirtyy tilitapahtumissa uudempiin tapahtumiin.
     */
    void btnNewerClicked();

    /**
     * @brief Siirtyy tilitapahtumissa vanhempiin tapahtumiin
     */
    void btnOlderClicked();


private:
    Ui::Transactions *ui;       ///< Käyttöliittymäolio

    QString token;              ///< Autentikaatio tokeni
    int accountId = -1;         ///< Tarkasteltavan tilin ID
    int currentOffset = 0;      ///< Haun aloituskohta (offset) sivutusta varten
    const int pageSize = 10;    ///< Sivukoko (tapahtumien määrä per sivu)
};

#endif // TRANSACTIONS_H

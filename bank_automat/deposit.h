#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDialog>

namespace Ui {
class Deposit;
}

/**
 * @class Deposit
 * @brief Ikkuna, jossa käyttäjä voi tallettaa rahaa valitsemalleen tilille.
 *
 * Deposit-dialogi mahdollistaa:
 * - talletussumman kasvattamisen valmiilla painikkeilla (5–500 €)
 * - summan nollaamisen
 * - talletuksen lähettämisen backendille
 * - onnistuneen talletuksen ilmoittamisen signaalilla
 */
class Deposit : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Luo Deposit-dialogin.
     * @param parent Vanhempi QWidget, yleensä nullptr.
     */
    explicit Deposit(QWidget *parent = nullptr);

    /**
     * @brief Tuhoaa Deposit-olion ja vapauttaa käyttöliittymän.
     */
    ~Deposit();

    /**
     * @brief Asettaa backend-tokenin, jota käytetään talletuspyynnössä.
     * @param t Token merkkijonona.
     */
    void setToken(const QString &t);

    /**
     * @brief Asettaa tilin ID:n, jolle talletus tehdään.
     * @param id Tilin tunniste.
     */
    void setAccountId(int id);

signals:
    /**
     * @brief Lähetetään, kun talletus on suoritettu onnistuneesti.
     */
    void depositSuccessful();

private slots:
    /**
     * @brief Palaa takaisin edelliseen näkymään.
     */
    void btnDepositBackClicked();

    /**
     * @brief Lisää 5 € talletussummaan.
     */
    void btnAddFiveClicked();

    /**
     * @brief Lisää 10 € talletussummaan.
     */
    void btnAddTenClicked();

    /**
     * @brief Lisää 20 € talletussummaan.
     */
    void btnAddTwentyClicked();

    /**
     * @brief Lisää 50 € talletussummaan.
     */
    void btnAddFiftyClicked();

    /**
     * @brief Lisää 100 € talletussummaan.
     */
    void btnAddHundredClicked();

    /**
     * @brief Lisää 200 € talletussummaan.
     */
    void btnAddTwoHundredClicked();

    /**
     * @brief Lisää 500 € talletussummaan.
     */
    void btnAddFiveHundredClicked();

    /**
     * @brief Nollaa talletussumman.
     */
    void btnResetClicked();

    /**
     * @brief Lähettää talletuspyynnön backendille.
     */
    void btnNewDepositClicked();

private:
    Ui::Deposit *ui;     ///< Käyttöliittymäolio

    QString token;       ///< Backend-token talletuspyyntöä varten
    int accountId = -1;  ///< Tili, jolle talletus tehdään
    int currentAmount = 0; ///< Tämänhetkinen talletussumma euroina

    /**
     * @brief Lähettää talletuspyynnön backendille.
     * @param amount Talletettava summa euroina.
     */
    void sendDepositRequest(int amount);
};

#endif // DEPOSIT_H

#ifndef TRANSFER_H
#define TRANSFER_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class transfer;
}

/**
 * @class transfer
 * @brief Ikkuna, jossa käyttäjä voi siirtää rahaa omalta tililtään toiselle tilille.
 *
 * Transfer-dialogi mahdollistaa:
 * - käyttäjän saldon hakemisen backendistä
 * - siirtosumman syöttämisen
 * - rahansiirron lähettämisen backendille
 * - onnistuneen siirron ilmoittamisen signaalilla
 */
class transfer : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Luo transfer-dialogin.
     * @param parent Vanhempi QWidget, yleensä nullptr.
     */
    explicit transfer(QWidget *parent = nullptr);

    /**
     * @brief Tuhoaa transfer-olion ja vapauttaa käyttöliittymän.
     */
    ~transfer();

    /**
     * @brief Asettaa tilin ID:n, jolta siirto tehdään.
     * @param id Tilin tunniste.
     */
    void setAccountId(int id);

    /**
     * @brief Asettaa backend-tokenin, jota käytetään siirtopyynnössä.
     * @param t Token merkkijonona.
     */
    void setToken(const QString &t);

    /**
     * @brief Hakee käyttäjän saldon backendistä.
     *
     * Tallentaa saldon @ref balance -muuttujaan.
     */
    void getBalance();

    QString balance; ///< Tilin tämänhetkinen saldo merkkijonona
    QString token;   ///< Backend-token siirtopyyntöä varten

signals:
    /**
     * @brief Lähetetään, kun rahansiirto on suoritettu onnistuneesti.
     */
    void transferSuccesful();

private slots:
    /**
     * @brief Palaa takaisin edelliseen näkymään.
     */
    void btnBackClicked();

    /**
     * @brief Lähettää rahansiirtopyynnön backendille.
     */
    void btnTransferMoneyClicked();

protected:
    /**
     * @brief Suoritetaan, kun dialogi näytetään ensimmäisen kerran.
     *
     * Tätä käytetään esimerkiksi saldon päivittämiseen automaattisesti.
     * @param event Näyttötapahtuma.
     */
    void showEvent(QShowEvent *event) override;

private:
    Ui::transfer *ui; ///< Käyttöliittymäolio

    int accountId = -1; ///< Tili, jolta siirto tehdään
};

#endif // TRANSFER_H

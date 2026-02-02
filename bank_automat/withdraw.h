#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QDialog>

namespace Ui {
class Withdraw;
}

/**
 * @class Withdraw
 * @brief Ikkuna, jossa käyttäjä voi nostaa rahaa tililtään.
 *
 * Withdraw-dialogi mahdollistaa:
 * - valmiiden nostosummien valitsemisen (20–100 €)
 * - noston lähettämisen backendille
 * - saldon näyttämisen
 * - onnistuneen noston ilmoittamisen signaalilla
 */
class Withdraw : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Luo Withdraw-dialogin.
     * @param parent Vanhempi QWidget, yleensä nullptr.
     */
    explicit Withdraw(QWidget *parent = nullptr);

    /**
     * @brief Tuhoaa Withdraw-olion ja vapauttaa käyttöliittymän.
     */
    ~Withdraw();

    QByteArray token;   ///< Backend-token nostopyyntöä varten
    int accountId;      ///< Tili, jolta nosto tehdään
    QString balance;    ///< Tilin tämänhetkinen saldo merkkijonona

signals:
    /**
     * @brief Lähetetään, kun nosto on suoritettu onnistuneesti.
     */
    void withdrawDone();

private slots:
    /**
     * @brief Palaa takaisin edelliseen näkymään.
     */
    void on_btnWithdrawBack_clicked();

    /**
     * @brief Nostaa 20 €.
     */
    void on_btn20_clicked();

    /**
     * @brief Nostaa 40 €.
     */
    void on_btn40_clicked();

    /**
     * @brief Nostaa 50 €.
     */
    void on_btn50_clicked();

    /**
     * @brief Nostaa 100 €.
     */
    void on_btn100_clicked();

    /**
     * @brief Lähettää nostopyynnön backendille käyttäjän valitsemalla summalla.
     */
    void on_btnNosta_clicked();

private:
    Ui::Withdraw *ui; ///< Käyttöliittymäolio

    /**
     * @brief Lähettää nostopyynnön backendille.
     * @param amount Nostettava summa euroina.
     */
    void sendWithdrawRequest(int amount);

protected:
    /**
     * @brief Suoritetaan, kun dialogi näytetään ensimmäisen kerran.
     *
     * Tätä käytetään esimerkiksi saldon päivittämiseen automaattisesti.
     * @param event Näyttötapahtuma.
     */
    void showEvent(QShowEvent *event) override;
};

#endif // WITHDRAW_H

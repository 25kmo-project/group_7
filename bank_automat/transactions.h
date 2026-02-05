#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class Transactions;
}

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
     * @brief Asettaa backend-tokenin, jota käytetään talletuspyynnössä.
     * @param t Token merkkijonona.
     */
    void setToken(const QString &t);

    /**
     * @brief Asettaa tilin ID:n, jolle talletus tehdään.
     * @param id Tilin tunniste.
     */
    void setAccountId(int id);


    void updateLogs();

private slots:
    /**
     * @brief Palaa takaisin edelliseen näkymään.
     */
    void btnCloseClicked();

    /**
     * @brief Selataan tapahtumia taaksepäin.
     */
    void btnPreviousClicked();

    /**
     * @brief Selataan tapahtumia eteenpäin.
     */
    void btnNextClicked();

    void getLogsSlot();

private:
    Ui::Transactions *ui;   ///< Käyttöliittymäolio

    QString token;       ///< Backend-token
    int accountId = -1;  ///< Tili, jolle talletus tehdään
    int currentOffset = 0;
    const int pageSize = 10;
};

#endif // TRANSACTIONS_H

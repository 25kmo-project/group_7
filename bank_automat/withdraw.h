#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>



namespace Ui {
class Withdraw;
}

class Withdraw : public QDialog
{
    Q_OBJECT

public:
    explicit Withdraw(QWidget *parent = nullptr);
    ~Withdraw();

    QString token;
    int accountId = -1; //Tarvitaan et account_id siirtyy ikkunalta ikkunalle!!!
    QString balance;

signals:
    void withdrawDone(); //Accountinfolle tieto

private slots:
    void on_btnWithdrawBack_clicked(); //Takaisin nappi

    void on_btn20_clicked(); //20€

    void on_btn40_clicked(); //40€

    void on_btn50_clicked(); //50€

    void on_btn100_clicked(); //100€

    void onWithdrawReply(QNetworkReply *reply); //Bäkkärin vastaus

    void on_btnNosta_clicked(); //nosto nappi

private:
    Ui::Withdraw *ui;
    QNetworkAccessManager *manager;
    //int lastAmount = 0;

    //int luku = 0;
    //void sendWithdraw(int amount);
protected:
    void showEvent(QShowEvent *event) override; //Saldon näyttöä varte

};

#endif // WITHDRAW_H

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
    int accountId = -1;
    QString balance;

signals:
    void withdrawDone();

private slots:
    void on_btnWithdrawBack_clicked();

    void on_btn20_clicked();

    void on_btn40_clicked();

    void on_btn50_clicked();

    void on_btn100_clicked();

    void onWithdrawReply(QNetworkReply *reply);

    void on_btnNosta_clicked();

private:
    Ui::Withdraw *ui;
    QNetworkAccessManager *manager;
    int lastAmount = 0;

    //int luku = 0;
    //void sendWithdraw(int amount);
protected:
    void showEvent(QShowEvent *event) override; //Saldon näyttöä varte

};

#endif // WITHDRAW_H

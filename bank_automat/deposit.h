#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include "environment.h"

namespace Ui {
class Deposit;
}

class Deposit : public QDialog
{
    Q_OBJECT

public:
    void setToken(const QString &t);
    void setAccountId(int id);
    explicit Deposit(QWidget *parent = nullptr);
    int currentAmount = 0;
    ~Deposit();

private:
    Ui::Deposit *ui;
    QNetworkAccessManager *manager;
    int accountId = -1;
    QString token;

private slots:
    void btnDepositBackClicked();
    void btnAddFiveClicked();
    void btnAddTenClicked();
    void btnAddTwentyClicked();
    void btnAddFiftyClicked();
    void btnAddHundredClicked();
    void btnAddTwoHundredClicked();
    void btnAddFiveHundredClicked();
    void btnNewDepositClicked();
    void btnResetClicked();
    void onDepositReply(QNetworkReply *reply);

signals:
    void depositSuccessful();
};

#endif // DEPOSIT_H

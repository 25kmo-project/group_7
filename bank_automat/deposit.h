#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "environment.h"

namespace Ui {
class Deposit;
}

class Deposit : public QDialog
{
    Q_OBJECT

public:
    explicit Deposit(QWidget *parent = nullptr);
    ~Deposit();

private:
    Ui::Deposit *ui;

    QNetworkAccessManager *manager;
    int currentAmount = 0;
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

signals:
    void depositSuccesful();
};

#endif // DEPOSIT_H

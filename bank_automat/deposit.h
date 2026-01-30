#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDialog>

namespace Ui {
class Deposit;
}

class Deposit : public QDialog
{
    Q_OBJECT

public:
    explicit Deposit(QWidget *parent = nullptr);
    ~Deposit();

    void setToken(const QString &t);
    void setAccountId(int id);

signals:
    void depositSuccessful();

private slots:
    void btnDepositBackClicked();
    void btnAddFiveClicked();
    void btnAddTenClicked();
    void btnAddTwentyClicked();
    void btnAddFiftyClicked();
    void btnAddHundredClicked();
    void btnAddTwoHundredClicked();
    void btnAddFiveHundredClicked();
    void btnResetClicked();
    void btnNewDepositClicked();

private:
    Ui::Deposit *ui;

    QString token;
    int accountId = -1;
    int currentAmount = 0;

    void sendDepositRequest(int amount);
};

#endif // DEPOSIT_H

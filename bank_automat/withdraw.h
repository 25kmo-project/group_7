#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QDialog>

namespace Ui {
class Withdraw;
}

class Withdraw : public QDialog
{
    Q_OBJECT

public:
    explicit Withdraw(QWidget *parent = nullptr);
    ~Withdraw();

    QByteArray token;
    int accountId;
    QString balance;

signals:
    void withdrawDone();

private slots:
    void on_btnWithdrawBack_clicked();
    void on_btn20_clicked();
    void on_btn40_clicked();
    void on_btn50_clicked();
    void on_btn100_clicked();
    void on_btnNosta_clicked();

private:
    Ui::Withdraw *ui;

    void sendWithdrawRequest(int amount);

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // WITHDRAW_H

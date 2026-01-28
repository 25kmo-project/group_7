#ifndef TRANSFER_H
#define TRANSFER_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class transfer;
}

class transfer : public QDialog
{
    Q_OBJECT

public:
    explicit transfer(QWidget *parent = nullptr);
    ~transfer();

    void setAccountId(int id);
    void setToken(const QString &t);

    void getBalance();
    QString balance;
    QString token;

signals:
    void transferSuccesful();

private slots:
    void btnBackClicked();
    void btnTransferMoneyClicked();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::transfer *ui;

    int accountId = -1;
};

#endif // TRANSFER_H

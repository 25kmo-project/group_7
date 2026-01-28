#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class Accountinfo;
}

class Accountinfo : public QDialog
{
    Q_OBJECT

public:
    explicit Accountinfo(QWidget *parent = nullptr);
    ~Accountinfo();

    void setUsername(const QString &newUsername);
    void setToken(const QByteArray &newToken);     // vain UI:lle
    void setAccountData(const QJsonObject &obj);   // täyttää labelit
    void setAccountType(const QString &type);
    void setAccountId(int id);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void backRequested();

private slots:
    void btnMyDataClicked();
    void btnWithdrawClicked();
    void btnNewDepositClicked();
    void btnTransferClicked();

    void MyDataSlot();            // saldo / tilitiedot
    void MyPersonalDataSlot();    // henkilötiedot
    void refreshBalance();        // päivitys noston/talletuksen jälkeen

    void on_btnBack_clicked();
    void on_btnLogout_clicked();

private:
    Ui::Accountinfo *ui;

    QString username;
    QByteArray token;     // vain UI:lle, API-kutsut käyttävät ApiClientiä
    QString accountType;
    int accountId = -1;
};

#endif // ACCOUNTINFO_H

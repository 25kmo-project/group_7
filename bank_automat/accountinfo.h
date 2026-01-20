#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "environment.h"
#include "data.h"
#include "choosecard.h"

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

    void setToken(const QByteArray &newToken);
    void setAccountData(const QJsonObject &obj);
    void setAccountType(const QString &type);
    //void setAccountType(const QString &type);
    void setAccountId(int id); //Tarvitaan et id siirtyy ikkunalta ikkunalle
protected:
    void showEvent(QShowEvent *event) override;




private:
    Ui::Accountinfo *ui;
    QString username;
    QByteArray token;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString accountType;
    int accountId = -1; //Tarvitaan et id siirtyy ikkunalta ikkunalle


private slots:
    void btnMyDataClicked();
    void btnWithdrawClicked();
    void MyDataSlot();
    void MyPersonalDataSlot();
    void handleNetworkError(QNetworkReply::NetworkError error);
    void refreshBalance(); //Päivittää automaattisesti accountinfon balancen

};

#endif // ACCOUNTINFO_H

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
protected:
    void showEvent(QShowEvent *event) override;




private:
    Ui::Accountinfo *ui;
    QString username;
    QByteArray token;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString accountType;

private slots:
    void btnMyDataClicked();
    void MyDataSlot(bool openData);
    void handleNetworkError(QNetworkReply::NetworkError error);

};

#endif // ACCOUNTINFO_H

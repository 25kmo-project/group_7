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

private:
    Ui::Accountinfo *ui;
    QString username;
    QByteArray token;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

private slots:
    void btnMyDataClicked();
    void MyDataSlot();


};

#endif // ACCOUNTINFO_H

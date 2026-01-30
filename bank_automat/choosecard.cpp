#include "choosecard.h"
#include "ui_choosecard.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include "apiclient.h"
#include "accountinfo.h"

ChooseCard::ChooseCard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChooseCard)
{
    ui->setupUi(this);

    connect(ui->btnDEBIT, &QPushButton::clicked, this, &ChooseCard::btnDEBITClicked);
    connect(ui->btnCREDIT, &QPushButton::clicked, this, &ChooseCard::btnCREDITClicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &ChooseCard::btnBackClicked);
}

ChooseCard::~ChooseCard()
{
    delete ui;
}

void ChooseCard::btnDEBITClicked()
{
    if (username.isEmpty()) {
        qDebug() << "ChooseCard: Username missing!";
        return;
    }

    QString endpoint = "bank_account/" + username + "/debit";

    auto reply = ApiClient::instance().get(endpoint);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);

        if (doc.isObject()) {
            openAccount(doc.object());
        } else {
            qDebug() << "ChooseCard: Invalid debit JSON";
        }

        reply->deleteLater();
    });
}

void ChooseCard::btnCREDITClicked()
{
    if (username.isEmpty()) {
        qDebug() << "ChooseCard: Username missing!";
        return;
    }

    QString endpoint = "bank_account/" + username + "/credit";

    auto reply = ApiClient::instance().get(endpoint);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);

        if (doc.isObject()) {
            openAccount(doc.object());
        } else {
            qDebug() << "ChooseCard: Invalid credit JSON";
        }

        reply->deleteLater();
    });
}

void ChooseCard::openAccount(const QJsonObject &obj)
{
    Accountinfo *acc = new Accountinfo(this);

    acc->setAccountData(obj);
    acc->setToken(ApiClient::instance().getToken());
    acc->setUsername(username);
    acc->setAccountId(obj["account_id"].toInt());
    acc->setAccountType(obj["account_type"].toString());

    acc->show();
    close();
}

void ChooseCard::setChooseCard(const QByteArray &newChooseCard)
{
    token = newChooseCard; // vain UI:lle, ei API-kutsuihin
}

void ChooseCard::setUsername(const QString &newUsername)
{
    username = newUsername;
}

void ChooseCard::btnBackClicked()
{
    emit backRequested();
    close();
}

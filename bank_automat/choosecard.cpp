#include "choosecard.h"
#include "environment.h"
#include "ui_choosecard.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "accountinfo.h"

ChooseCard::ChooseCard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChooseCard)
{
    manager = new QNetworkAccessManager(this);
    ui->setupUi(this);
    connect(ui->btnDEBIT, &QPushButton::clicked, this, &ChooseCard::btnDEBITClicked);
    connect(ui->btnCREDIT, &QPushButton::clicked, this, &ChooseCard::btnCREDITClicked);
}

ChooseCard::~ChooseCard()
{
    delete ui;
}

void ChooseCard::btnDEBITClicked()
{
    if (username.isEmpty() || token.isEmpty()) {
        qDebug() << "ChooseCard: Username or token is empty! Cannot fetch accounts.";
        return;
    }

    QString url = Environment::base_url() + "bank_account/" + username;
    qDebug() << "ChooseCard: Fetching debit accounts from URL:" << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray myToken = "Bearer " + token;
    request.setRawHeader(QByteArray("Authorization"), myToken);
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ChooseCard::handleDebit);
    connect(reply, &QNetworkReply::errorOccurred, this, &ChooseCard::handleNetworkError);
}

void ChooseCard::btnCREDITClicked()
{
    if (username.isEmpty() || token.isEmpty()) {
        qDebug() << "ChooseCard: Username or token is empty! Cannot fetch accounts.";
        return;
    }

    QString url = Environment::base_url() + "bank_account/" + username;
    qDebug() << "ChooseCard: Fetching credit accounts from URL:" << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray myToken = "Bearer " + token;
    request.setRawHeader(QByteArray("Authorization"), myToken);
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ChooseCard::handleCredit);
    connect(reply, &QNetworkReply::errorOccurred, this, &ChooseCard::handleNetworkError);
}

void ChooseCard::handleDebit()
{
    qDebug() << "handleDebit() called";
    QByteArray response = reply->readAll();
    qDebug() << "Raw response:" << response;
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull()) {
        qDebug() << "ChooseCard: Invalid JSON in debit response";
        return;
    }

    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        qDebug() << "Single account object found:" << obj;
        if (obj["account_type"].toString() == "debit" && obj["user_id"].toInt() == username.toInt()) {
            qDebug() << "DEBIT selected (single object):" << obj;
            openAccount(obj);
            return;
        }
    } else if (doc.isArray()) {
        QJsonArray arr = doc.array();
        for (auto it = arr.constBegin(); it != arr.constEnd(); ++it) {
            QJsonObject obj = it->toObject();
            qDebug() << "Checking account in array:" << obj;
            if (obj["account_type"].toString() == "debit" && obj["user_id"].toInt() == username.toInt()) {
                qDebug() << "DEBIT selected (from array):" << obj;
                openAccount(obj);
                return;
            }
        }
    }

    qDebug() << "Debit-tiliä ei löytynyt!";
}

void ChooseCard::handleCredit()
{
    qDebug() << "handleCredit() called";
    QByteArray response = reply->readAll();
    qDebug() << "Raw response:" << response;
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull()) {
        qDebug() << "ChooseCard: Invalid JSON in credit response";
        return;
    }


    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        qDebug() << "Single account object found:" << obj;
        if (obj["account_type"].toString() == "credit" && obj["user_id"].toInt() == username.toInt()) {
            qDebug() << "CREDIT selected (single object):" << obj;
            openAccount(obj);
            return;
        }
    } else if (doc.isArray()) {
        QJsonArray arr = doc.array();

        for (auto it = arr.constBegin(); it != arr.constEnd(); ++it) {
            QJsonObject obj = it->toObject();
            qDebug() << "Checking account in array:" << obj;
            if (obj["account_type"].toString() == "credit" && obj["user_id"].toInt() == username.toInt()) {
                qDebug() << "CREDIT selected (from array):" << obj;
                openAccount(obj);
                return;
            }
        }
    }

    qDebug() << "Credit-tiliä ei löytynyt!";
}

void ChooseCard::openAccount(const QJsonObject &obj)
{
    Accountinfo *acc = new Accountinfo(this);
    acc->setAccountData(obj);
    acc->setToken(token);
    acc->setUsername(username);
    acc->show();
    this->close();
}

void ChooseCard::setChooseCard(const QByteArray &newChooseCard)
{
    token = newChooseCard;
    qDebug() << "ChooseCard: Token set to" << token;
}

void ChooseCard::setUsername(const QString &newUsername)
{
    username = newUsername;
    qDebug() << "ChooseCard: Username set to" << username;
}

void ChooseCard::handleNetworkError(QNetworkReply::NetworkError error)
{
    qDebug() << "ChooseCard: Network error:" << error << "-" << reply->errorString();
}



/*void ChooseCard::on_btn_Credit_clicked()
{
    //cardSelected("credit");
    qDebug() << "Credit painettu";
    qDebug() << "Debit painettu";
    QString url = Environment::base_url() + "bank_account";
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + token);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &ChooseCard::debitTiedot);
    manager->get(request);

}


void ChooseCard::on_btn_Debit_clicked()
{
    //cardSelected("Debit");
    qDebug() << "Debit painettu";
    QString url = Environment::base_url() + "bank_account";
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + token);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
    this, &ChooseCard::debitTiedot);
    manager->get(request);

}

void ChooseCard::debitTiedot(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    qDebug() <<"ID: " << obj["account_id"].toInt();
    qDebug() << "Type:" << obj["account_type"].toString();
    qDebug() << "Number:" << obj["account_number"].toString();
    qDebug() << "Balance:" << obj["balance"].toString();
    qDebug() << "Credit limit:" << obj["credit_limit"].toString();
    qDebug() << "User ID:" << obj["user_id"].toInt();



    //ui->textDebug->setText(debug);

    reply->deleteLater();
}

void ChooseCard::creditTiedot(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    qDebug() <<"ID: " << obj["account_id"].toInt();
    qDebug() << "Type:" << obj["account_type"].toString();
    qDebug() << "Number:" << obj["account_number"].toString();
    qDebug() << "Balance:" << obj["balance"].toString();
    qDebug() << "Credit limit:" << obj["credit_limit"].toString();
    qDebug() << "User ID:" << obj["user_id"].toInt();

}

*/

#include "accountinfo.h"
#include "ui_accountinfo.h"
#include <QShowEvent>
#include "data.h"  // Lisää tämä, jos Data on eri headerissa

Accountinfo::Accountinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Accountinfo)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(ui->btnMyData, &QPushButton::clicked, this, &Accountinfo::btnMyDataClicked);
}

Accountinfo::~Accountinfo()
{
    delete ui;
}

void Accountinfo::setAccountType(const QString &type)
{
    accountType = type;
    qDebug() << "Accountinfo: account type set to" << accountType;
}

/*void Accountinfo::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    if (username.isEmpty() || token.isEmpty() || accountType.isEmpty()) {
        qDebug() << "Accountinfo: Username, token or accountType empty! Cannot fetch saldo.";
        return;
    }
    QString url = Environment::base_url() + "bank_account/" + username + "/" + accountType;
    qDebug() << "Accountinfo: Fetching saldo automatically from URL:" << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray myToken = "Bearer " + token;
    request.setRawHeader("Authorization", myToken);
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Accountinfo::MyDataSlot);
}*/

void Accountinfo::setUsername(const QString &newUsername)
{
    username = newUsername;
    ui->labelUsername->setText(username);
    qDebug() << "Accountinfo: Username set to" << username;
}

void Accountinfo::setToken(const QByteArray &newToken)
{
    token = newToken;
    qDebug() << "Accountinfo: Token set to" << token;
}

void Accountinfo::setAccountData(const QJsonObject &obj)
{
    qDebug() << "setAccountData called with obj:" << obj;
    if (obj.contains("account_id")) {
        ui->labelID->setText(QString::number(obj["account_id"].toInt()));
        qDebug() << "labelID set to:" << ui->labelID->text();
    } else {
        ui->labelID->setText("Ei dataa");
        qDebug() << "account_id missing!";
    }
    if (obj.contains("account_type")) {
        ui->labelType->setText(obj["account_type"].toString());
        qDebug() << "labelType set to:" << ui->labelType->text();
    } else {
        ui->labelType->setText("Ei dataa");
    }
    if (obj.contains("account_number")) {
        ui->labelNumber->setText(obj["account_number"].toString());
        qDebug() << "labelNumber set to:" << ui->labelNumber->text();
    } else {
        ui->labelNumber->setText("Ei dataa");
    }
    if (obj.contains("balance")) {
        ui->labelBalance->setText(obj["balance"].toString());
        qDebug() << "labelBalance set to:" << ui->labelBalance->text();
    } else {
        ui->labelBalance->setText("Ei dataa");
    }
    if (obj.contains("credit_limit")) {
        ui->labelCreditLimit->setText(obj["credit_limit"].toString());
        qDebug() << "labelCreditLimit set to:" << ui->labelCreditLimit->text();
    } else {
        ui->labelCreditLimit->setText("Ei dataa");
    }
    if (obj.contains("user_id")) {
        ui->labelUserID->setText(QString::number(obj["user_id"].toInt()));
        qDebug() << "labelUserID set to:" << ui->labelUserID->text();
    } else {
        ui->labelUserID->setText("Ei dataa");
    }
    this->update(); // Pakota UI-päivitys
}

void Accountinfo::btnMyDataClicked()
{
    if (username.isEmpty() || token.isEmpty()) {
        qDebug() << "Accountinfo: Username or token empty! Cannot fetch personal data.";
        return;
    }
    QString url = Environment::base_url() + "bank_user/" + username;
    qDebug() << "Accountinfo: Fetching personal data from URL:" << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray myToken = "Bearer " + token;
    request.setRawHeader("Authorization", myToken);
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this]() { MyDataSlot(true); }); // Lambda: Välitä true
    connect(reply, &QNetworkReply::errorOccurred, this, &Accountinfo::handleNetworkError);
}

void Accountinfo::MyDataSlot(bool openData)
{
    QByteArray response = reply->readAll();
    qDebug() << "Accountinfo: Response from backend:" << response;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        setAccountData(jsonDoc.object()); // Päivitä labelit aina (esim. saldo automaattisesti)
        qDebug() << "Accountinfo: Data parsed and set successfully";
        if (openData) { // Vain jos true (napista), avaa Data-ikkuna henkilötiedoilla
            Data *objData = new Data(this);
            objData->setTestData(response);
            objData->show();
        }
    } else {
        qDebug() << "Accountinfo: Invalid JSON response";
    }
    reply->deleteLater();
}

void Accountinfo::handleNetworkError(QNetworkReply::NetworkError error)
{
    qDebug() << "Accountinfo: Network error:" << error << "-" << reply->errorString();
}

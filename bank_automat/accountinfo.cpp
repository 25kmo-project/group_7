#include "accountinfo.h"
#include "build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug/bank_automat_autogen/include/ui_accountinfo.h"
#include "ui_accountinfo.h"
#include <QShowEvent>
#include "data.h"  // Lisää tämä, jos Data on eri headerissa
#include "withdraw.h"
#include "deposit.h"

Accountinfo::Accountinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Accountinfo)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(ui->btnMyData, &QPushButton::clicked, this, &Accountinfo::btnMyDataClicked);
    connect(ui->btnWithdraw, &QPushButton::clicked,this, &Accountinfo :: btnWithdrawClicked);
    connect(ui->btnNewDeposit, &QPushButton::clicked, this, &Accountinfo::btnNewDepositClicked);
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



void Accountinfo::setUsername(const QString &newUsername)
{
    username = newUsername;

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
        accountId = obj["account_id"].toInt();
        ui->labelID->setText(QString::number(obj["account_id"].toInt()));
        qDebug() << "labelID set to:" << ui->labelID->text();
    } else {
        ui->labelID->setText("Ei dataa");
        qDebug() << "account_id missing!";
    }

    // 1. Tilityyppi → labelType
    if (obj.contains("account_type")) {
        ui->labelType->setText(obj["account_type"].toString());
        qDebug() << "labelType set to:" << ui->labelType->text();
    } else {
        ui->labelType->setText("Ei dataa");
    }

    // 2. Tilinumero → labelAccountNumber
    if (obj.contains("account_number")) {
        ui->labelAccountNumber->setText(obj["account_number"].toString());
        qDebug() << "labelAccountNumber set to:" << ui->labelAccountNumber->text();
    } else {
        ui->labelAccountNumber->setText("Ei dataa");
    }

    // 3. Saldo → labelBalance
    if (obj.contains("balance")) {
        ui->labelBalance->setText(obj["balance"].toString());
        qDebug() << "labelBalance set to:" << ui->labelBalance->text();
    } else {
        ui->labelBalance->setText("Ei dataa");
    }

    // 4. Luottoraja → labelCreditLimit
    if (obj.contains("credit_limit")) {
        ui->labelCreditLimit->setText(obj["credit_limit"].toString());
        qDebug() << "labelCreditLimit set to:" << ui->labelCreditLimit->text();
    } else {
        ui->labelCreditLimit->setText("Ei dataa");
    }

    this->update(); // Pakota UI-päivitys
}



void Accountinfo::showEvent(QShowEvent *event)
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
    connect(reply, &QNetworkReply::finished, this, &Accountinfo::MyDataSlot);  // Käytä vanhaa slottia saldon päivitykseen
    connect(reply, &QNetworkReply::errorOccurred, this, &Accountinfo::handleNetworkError);
}

void Accountinfo::setAccountId(int id) //Tallettaa id ikkunalta ikkunalle jne.
{
    accountId = id;
    qDebug() << "Accountinfo: accountId set to" << accountId;


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
    connect(reply, &QNetworkReply::finished, this, &Accountinfo::MyPersonalDataSlot);  // Uusi slotti vain henkilötiedoille ja Data-ikkunalle
    connect(reply, &QNetworkReply::errorOccurred, this, &Accountinfo::handleNetworkError);
}
void Accountinfo::btnWithdrawClicked() //Nosto-nappi päävalikossa
{
    qDebug() << "DEBUG: btnWithdrawClicked, accountId =" << accountId;
    Withdraw *objWd = new Withdraw(this); //Luo nosto ikkunan
    objWd->token = this->token; //Annetaa token nosto ikkunalle
    objWd->accountId = this->accountId; //Annetaan accountid nosto ikkunalle
    objWd->balance = ui->labelBalance->text();
    connect(objWd,&Withdraw::withdrawDone, this, &Accountinfo::refreshBalance); //Tässä yhdistyy accountinfo ja refresh
    objWd->show();
}

void Accountinfo::btnNewDepositClicked()
{
    Deposit *objDeposit = new Deposit(this);
    objDeposit->show();
}

void Accountinfo::MyDataSlot()  // Vanha slotti: Vain saldon päivitys
{
    QByteArray response = reply->readAll();
    qDebug() << "Accountinfo: Response from backend (saldo):" << response;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        setAccountData(jsonDoc.object());  // Päivitä Accountinfo:n labelit (saldo jne.)
        qDebug() << "Accountinfo: Saldo data parsed and set successfully";
        Data *objData = new Data(this);
        connect(objData, &Data::logoutRequested, this, &Accountinfo::close);  // signaali sulkemiseen
        objData->setTestData(response);
        objData->show();
    } else {
        qDebug() << "Accountinfo: Invalid JSON for saldo";
    }
    reply->deleteLater();
}

void Accountinfo::MyPersonalDataSlot()  // Uusi slotti: Henkilötiedot ja Data-ikkuna
{
    QByteArray response = reply->readAll();
    qDebug() << "Accountinfo: Response from backend (personal):" << response;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        qDebug() << "Accountinfo: Personal data parsed successfully";

        // Avaa Data-ikkuna (ei päivitä Accountinfo:n label:eitä)
        Data *objData = new Data(this);
        connect(objData, &Data::logoutRequested, this, &Accountinfo::close);
        objData->setTestData(response);
        objData->show();
    } else {
        qDebug() << "Accountinfo: Invalid JSON for personal data";
        // QMessageBox::warning(this, "Virhe", "Ei henkilötietoja saatavilla.");
    }
    reply->deleteLater();
}

void Accountinfo::handleNetworkError(QNetworkReply::NetworkError error)
{
    qDebug() << "Accountinfo: Network error:" << error << "-" << reply->errorString();
}

void Accountinfo::refreshBalance() //emit withDraw(); done lähettää tänne signaalin
{
    //qDebug() << "Refreshing balance after withdraw...";

    QString url = Environment::base_url() + "bank_account/" + username + "/" + accountType;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + token);

    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Accountinfo::MyDataSlot); //Päivittää saldon ui:hin.
}


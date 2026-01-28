#include "accountinfo.h"
#include "ui_accountinfo.h"

#include <QShowEvent>
#include <QJsonDocument>
#include <QJsonObject>

#include "apiclient.h"
#include "data.h"
#include "withdraw.h"
#include "deposit.h"
#include "transfer.h"

Accountinfo::Accountinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Accountinfo)
{
    ui->setupUi(this);

    connect(ui->btnMyData, &QPushButton::clicked, this, &Accountinfo::btnMyDataClicked);
    connect(ui->btnWithdraw, &QPushButton::clicked, this, &Accountinfo::btnWithdrawClicked);
    connect(ui->btnDeposit, &QPushButton::clicked, this, &Accountinfo::btnNewDepositClicked);
    connect(ui->btnTransfer, &QPushButton::clicked, this, &Accountinfo::btnTransferClicked);
}

Accountinfo::~Accountinfo()
{
    delete ui;
}

void Accountinfo::setAccountType(const QString &type)
{
    accountType = type;
}

void Accountinfo::setUsername(const QString &newUsername)
{
    username = newUsername;
}

void Accountinfo::setToken(const QByteArray &newToken)
{
    token = newToken; // tämä on Withdraw/Deposit/Transfer ikkunoita varten
}


void Accountinfo::setAccountId(int id)
{
    accountId = id;
}
void Accountinfo::setAccountData(const QJsonObject &obj)
{
    if (obj.contains("account_id")) {
        accountId = obj["account_id"].toInt();
        ui->labelID->setText(QString::number(accountId));
    }

    ui->labelType->setText(obj.value("account_type").toString("Ei dataa"));
    ui->labelAccountNumber->setText(obj.value("account_number").toString("Ei dataa"));
    ui->labelBalance->setText(obj.value("balance").toString("Ei dataa"));
    ui->labelCreditLimit->setText(obj.value("credit_limit").toString("Ei dataa"));
}

void Accountinfo::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    if (username.isEmpty() || accountType.isEmpty()) {
        qDebug() << "Accountinfo: Missing username or accountType";
        return;
    }

    QString endpoint = "bank_account/" + username + "/" + accountType;

    auto reply = ApiClient::instance().get(endpoint);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (doc.isObject()) {
            setAccountData(doc.object());
        }

        reply->deleteLater();
    });
}

void Accountinfo::btnMyDataClicked()
{
    QString endpoint = "bank_user/" + username;

    auto reply = ApiClient::instance().get(endpoint);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (doc.isObject()) {
            Data *objData = new Data(this);
            objData->setTestData(data);
            connect(objData, &Data::logoutRequested, this, &Accountinfo::close);
            objData->show();
        }

        reply->deleteLater();
    });
}

void Accountinfo::btnWithdrawClicked()
{
    Withdraw *objWd = new Withdraw(this);
    objWd->token = token;
    objWd->accountId = accountId;
    objWd->balance = ui->labelBalance->text();

    connect(objWd, &Withdraw::withdrawDone, this, &Accountinfo::refreshBalance);
    objWd->show();
}

void Accountinfo::btnNewDepositClicked()
{
    Deposit *objDeposit = new Deposit(this);
    objDeposit->setToken(QString(token));
    objDeposit->setAccountId(accountId);

    connect(objDeposit, &Deposit::depositSuccessful, this, &Accountinfo::refreshBalance);
    objDeposit->show();
}

void Accountinfo::btnTransferClicked()
{
    transfer *objTransfer = new transfer(this);
    objTransfer->setToken(QString(token));
    objTransfer->setAccountId(accountId);
    objTransfer->balance = ui->labelBalance->text();

    connect(objTransfer, &transfer::transferSuccesful, this, &Accountinfo::refreshBalance);
    objTransfer->show();
}

void Accountinfo::refreshBalance()
{
    QString endpoint = "bank_account/" + QString::number(accountId);

    auto reply = ApiClient::instance().get(endpoint);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (doc.isObject()) {
            setAccountData(doc.object());
        }

        reply->deleteLater();
    });
}

void Accountinfo::on_btnBack_clicked()
{
    emit backRequested();
    close();
}

void Accountinfo::on_btnLogout_clicked()
{
    close();
}


// void Accountinfo::MyDataSlot()
// {
//     //Ei ole enään käytössä mutta MOC vaatii tämän olemassa olon(?)
// }

#include "environment.h"
#include "transfer.h"
#include "ui_transfer.h"
#include <QMessageBox>
#include <qevent.h>


transfer::transfer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::transfer)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, &transfer::onTransferReply);
    connect(ui->btnTransferBack, &QPushButton::clicked, this, &transfer::btnBackClicked);
    connect(ui->btnTransferMoney, &QPushButton::clicked, this, &transfer::btnTransferMoneyClicked);

}

transfer::~transfer()
{
    delete ui;
}

void transfer::setAccountId(int id)
{
    accountId = id;
}

void transfer::setToken(const QString &t)
{
    token = t;
}

void transfer::onTransferReply(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    qDebug() << "WITHDRAW RESPONSE:" << data;
}

void transfer::btnBackClicked()
{
    emit transferSuccesful();
    this->close();
}

void transfer::btnTransferMoneyClicked()
{
    qDebug() << "nappia painettu";
    QNetworkRequest req(Environment::base_url() + "bank_log/transfer");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["first_account"] = accountId;

    // Poista lopusta << .toIint() >> kun tietokannan transfer funktio on muutettu käyttämään tilinumeroa eikä accountID
    json["second_account"] = ui->textTransferAccountNumber->text().toInt();

    json["amount"] = ui->textTrasferAmount->text().toDouble();
    qDebug() << "amount = " << json["amount"].toDouble();

    manager->post(req, QJsonDocument(json).toJson());



}

// siirto-ikkunan saldo
void transfer::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    qDebug() << "WITHDRAW RESPONSE:" << balance;

    ui->labelTransferAccountBalance->setText("Saldo: " + balance + " €");
}

#include "transfer.h"
#include "ui_transfer.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

#include "apiclient.h"

transfer::transfer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::transfer)
{
    ui->setupUi(this);

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
    token = t; // vain UI:lle, ei API-kutsuihin
}

// -----------------------------
//  HAE SALDO
// -----------------------------
void transfer::getBalance()
{
    QString endpoint = "bank_account/" + QString::number(accountId);

    auto reply = ApiClient::instance().get(endpoint);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);

        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            balance = obj.value("balance").toString();
            ui->labelTransferAccountBalance->setText(balance + " €");
        }

        reply->deleteLater();
    });
}

// -----------------------------
//  SIIRTO
// -----------------------------
void transfer::btnTransferMoneyClicked()
{
    int targetAccount = ui->textTransferAccountNumber->text().toInt();
    double amount = ui->textTrasferAmount->text().toDouble();

    if (targetAccount <= 0 || amount <= 0) {
        QMessageBox::warning(this, "Virhe", "Syötä kelvollinen tili ja summa.");
        return;
    }

    QJsonObject json;
    json["first_account"] = accountId;
    json["second_account"] = targetAccount;
    json["amount"] = amount;

    auto reply = ApiClient::instance().post("bank_log/transfer", json);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        qDebug() << "TRANSFER RESPONSE:" << response;

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Virhe siirrossa", reply->errorString());
            reply->deleteLater();
            return;
        }

        // Siirto onnistui → päivitä saldo
        getBalance();

        QMessageBox::information(this, "Siirto onnistui", "Rahansiirto suoritettu.");
        emit transferSuccesful();

        reply->deleteLater();
    });
}

// -----------------------------
//  UI
// -----------------------------
void transfer::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->labelTransferAccountBalance->setText(balance + " €");
}

void transfer::btnBackClicked()
{
    close();
}

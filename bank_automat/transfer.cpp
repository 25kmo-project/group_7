#include "transfer.h"
#include "ui_transfer.h"


transfer::transfer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::transfer)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);

    // ui->labelTransferAccountBalance->setText("");

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

void transfer::getBalance()
{
    QNetworkRequest req(Environment::base_url() + "bank_account/" + QString::number(accountId));
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
    manager->get(req);
}


void transfer::onTransferReply(QNetworkReply *reply)
{
    // päivitetään siirto ikkunan saldo *

    QByteArray response = reply->readAll();
    qDebug() << "RESPONSEEEEEE:" << response;

    // haetaan uusin balance
    if (reply->url().toString().contains("bank_log/transfer")) {
        getBalance();
        reply->deleteLater();
        return;
    }

    // jos löytyy nii päivitetää balance
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject obj = doc.object();
    //  qDebug() << "BALANCE IS =  :" << obj["balance"];

    if (obj.contains("balance")) {
        balance = obj["balance"].toString();
        ui->labelTransferAccountBalance->setText(balance + " €");
    }


    emit transferSuccesful();
    reply->deleteLater();
}

void transfer::btnBackClicked()
{
    this->close();
}

void transfer::btnTransferMoneyClicked()
{
    // POST-pyyntö backendille
    QNetworkRequest req(Environment::base_url() + "bank_log/transfer");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["first_account"] = accountId;

    // Poista lopusta << .toIint() >> kun tietokannan transfer funktio on muutettu käyttämään tilinumeroa eikä accountID
    json["second_account"] = ui->textTransferAccountNumber->text().toInt();

    // qDebug() << "amount = " << json["amount"].toDouble();
    json["amount"] = ui->textTrasferAmount->text().toDouble();

    manager->post(req, QJsonDocument(json).toJson());

}

// siirto-ikkunan saldo
void transfer::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    // qDebug() << "TRANSFER RESPONSE:" << balance;

    ui->labelTransferAccountBalance->setText(balance + " €");
}

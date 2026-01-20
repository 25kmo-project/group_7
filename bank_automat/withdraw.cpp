#include "withdraw.h"
#include "ui_withdraw.h"
#include "environment.h"

#include <QNetworkReply>

Withdraw::Withdraw(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Withdraw)
{
    ui->setupUi(this);


    //ui->labelBalance->setText("Saldo: " + balance + " €");
    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished,this, &Withdraw::onWithdrawReply);




}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::on_btnWithdrawBack_clicked()
{
    qDebug() << "Paluu päävalikkoon";
    emit withdrawDone();
    this->close();


}


void Withdraw::on_btn20_clicked()
{

    QNetworkRequest req(Environment::base_url() + "bank_log/withdraw");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = 20;
    qDebug() << "Withdraw: accountId =" << accountId;


    manager->post(req, QJsonDocument(json).toJson());
}



void Withdraw::on_btn40_clicked()
{
    QNetworkRequest req(Environment::base_url() + "bank_log/withdraw");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = 40;
    qDebug() << "Withdraw: accountId =" << accountId;


    manager->post(req, QJsonDocument(json).toJson());
}


void Withdraw::on_btn50_clicked()
{
    QNetworkRequest req(Environment::base_url() + "bank_log/withdraw");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = 50;
    qDebug() << "Withdraw: accountId =" << accountId;


    manager->post(req, QJsonDocument(json).toJson());
}
void Withdraw::on_btn100_clicked()
{
    QNetworkRequest req(Environment::base_url() + "bank_log/withdraw");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = 100;
    qDebug() << "Withdraw: accountId =" << accountId;


    manager->post(req, QJsonDocument(json).toJson());
}
void Withdraw::onWithdrawReply(QNetworkReply *reply)
{

    QByteArray response = reply->readAll();
    qDebug() << "WITHDRAW RESPONSE:" << response;


    if (reply->error() != QNetworkReply::NoError) {

        reply->deleteLater();
        return;
    }


    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject obj = doc.object();


    if (obj.contains("error")) {

        reply->deleteLater();
        return;
    }






  reply->deleteLater();
}

void Withdraw::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->labelBalance->setText("Saldo: " + balance + " €");

}


void Withdraw::on_btnNosta_clicked()
{
    QString text = ui->lineEditAmount->text();
    int amount = text.toInt();

    // 1. Perustarkistukset
    if (amount < 20) {
        ui->labelError->setText("Miniminosto on 20 €");
        return;
    }

    if (amount % 10 != 0) {
        ui->labelError->setText("Summa pitää olla 10 € tarkkuudella");
        return;
    }

    // 2. Tarkista voiko summan muodostaa 20 ja 50 seteleillä
    bool possible = false;

    for (int fifties = 0; fifties * 50 <= amount; fifties++) {
        int remaining = amount - fifties * 50;
        if (remaining % 20 == 0) {
            possible = true;
            break;
        }
    }

    if (!possible) {
        ui->labelError->setText("Summaa ei voi muodostaa 20 € ja 50 € seteleillä");
        return;
    }

    // 3. Lähetä nosto backendille
    lastAmount = amount;

    QNetworkRequest req(Environment::base_url() + "bank_log/withdraw");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = amount;

    manager->post(req, QJsonDocument(json).toJson());
}



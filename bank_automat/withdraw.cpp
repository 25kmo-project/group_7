#include "withdraw.h"
#include "ui_withdraw.h"
#include "environment.h"
#include <QMessageBox>


#include <QNetworkReply>

Withdraw::Withdraw(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Withdraw)
{
    ui->setupUi(this);

    // HTTP-manageri joka hoitaa backend-kutsut
    manager = new QNetworkAccessManager(this);

    // Kun backend vastaa POST-pyyntöön → kutsutaan onWithdrawReply()
    connect(manager, &QNetworkAccessManager::finished, this, &Withdraw::onWithdrawReply);
}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::on_btnWithdrawBack_clicked()
{
    // Käyttäjä palaa takaisin → ilmoitetaan Accountinfolle
    emit withdrawDone();

    // Suljetaan ikkuna
    this->close();
}

//Nosto-napit

void Withdraw::on_btn20_clicked()
{
    // Luodaan POST-pyyntö backendille
    QNetworkRequest req(Environment::base_url() + "bank_log/withdraw");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    // JSON-data nostoa varten
    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = 20;

    // Lähetetään pyyntö
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

    manager->post(req, QJsonDocument(json).toJson());
}


//Backendin vastaus pyyntöön

void Withdraw::onWithdrawReply(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    qDebug() << "WITHDRAW RESPONSE:" << response;


    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Virhe nostossa", "Verkkovirhe tapahtui.");
        reply->deleteLater();
        return;
    }


    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject obj = doc.object();

    // 3. Backend virhe
    if (obj.contains("sqlMessage")) {
        QString err = obj["sqlMessage"].toString();
        QMessageBox::warning(this, "Virhe nostossa", err);
        qDebug() << "DEBUG: BACKEND-VIRHE, EI SULJETA IKKUNAA";

        reply->deleteLater();
        return;
    }

    //Onnistui
    emit withdrawDone();
    this->close();

    reply->deleteLater();
}



// Näyttää nosto-ikkunan saldon

void Withdraw::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);


    ui->labelBalance->setText("Saldo: " + balance + " €");
}


// Muu summa

void Withdraw::on_btnNosta_clicked()
{
    QString text = ui->lineEditAmount->text();
    int amount = text.toInt();


    if (amount < 20) {
        ui->labelError->setText("Miniminosto on 20 €");
        return;
    }


    if (amount % 10 != 0) {
        ui->labelError->setText("Summa pitää olla 10 € tarkkuudella");
        return;
    }


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

    //Nosto backendille
    //lastAmount = amount;

    QNetworkRequest req(Environment::base_url() + "bank_log/withdraw");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = amount;

    manager->post(req, QJsonDocument(json).toJson());
}

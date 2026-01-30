#include "withdraw.h"
#include "ui_withdraw.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

#include "apiclient.h"

Withdraw::Withdraw(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Withdraw)
{
    ui->setupUi(this);
}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::on_btnWithdrawBack_clicked()
{
    emit withdrawDone();
    close();
}

// -----------------------------
//  YHTEINEN FUNKTIO NOSTOILLE
// -----------------------------
void Withdraw::sendWithdrawRequest(int amount)
{
    QJsonObject json;
    json["oma_account_id"] = accountId;
    json["maara_amount"] = amount;

    auto reply = ApiClient::instance().post("bank_log/withdraw", json);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        qDebug() << "WITHDRAW RESPONSE:" << response;

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Virhe nostossa", "Verkkovirhe tapahtui.");
            reply->deleteLater();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        if (obj.contains("sqlMessage")) {
            QMessageBox::warning(this, "Virhe nostossa", obj["sqlMessage"].toString());
            reply->deleteLater();
            return;
        }

        QMessageBox::information(this, "Nosto onnistui", "Nosto suoritettu onnistuneesti.");
        emit withdrawDone();
        close();

        reply->deleteLater();
    });
}

// -----------------------------
//  NOSTONAPIT
// -----------------------------
void Withdraw::on_btn20_clicked()
{
    sendWithdrawRequest(20);
}

void Withdraw::on_btn40_clicked()
{
    sendWithdrawRequest(40);
}

void Withdraw::on_btn50_clicked()
{
    sendWithdrawRequest(50);
}

void Withdraw::on_btn100_clicked()
{
    sendWithdrawRequest(100);
}

// -----------------------------
//  Muu summa
// -----------------------------
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

    sendWithdrawRequest(amount);
}

// -----------------------------
//  UI
// -----------------------------
void Withdraw::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->labelBalance->setText("Saldo: " + balance + " €");
}

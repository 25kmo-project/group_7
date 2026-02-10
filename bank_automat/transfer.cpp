/**
 * @file Transfer.cpp
 * @brief Rahansiirron dialogin toteutus.
 * @details Käyttäjä voi syöttää kohdetilinumeron ja summan, siirto lähetetään backendille ja saldo päivitetään onnitumisen jälkeen.
 */

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
    //connectit napile
    connect(ui->btnTransferBack, &QPushButton::clicked, this, &transfer::btnBackClicked);
    connect(ui->btnTransferMoney, &QPushButton::clicked, this, &transfer::btnTransferMoneyClicked);
    connect(ui->textTrasferAmount, &QLineEdit::returnPressed, this, &transfer::btnTransferMoneyClicked);
}

transfer::~transfer()
{
    delete ui;
}
/**
 * @brief Asettaa tilin ID:n josta siirto lähetetään.
 * @param id Oma account_id
 */
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

/**
 * @brief Hakee nykyisen tilin saldon backendistä ja näyttää sen.
 * @details Kutsutaan aina kun ikkuna avataan ja onnistuneen siirron jälkeen.
 */
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

/**
 * @brief Käsittelee "Siirrä" napin klikkauksen
 * @details Validoi syötteet ja lähettää POST:n ja päivittää saldon onnistuessa.
 */
void transfer::btnTransferMoneyClicked()
{
    QString targetAccount = ui->textTransferAccountNumber->text().trimmed();
    double amount = ui->textTrasferAmount->text().toDouble();

    if (targetAccount.isEmpty() || amount <= 0) {
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

        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        if (obj.contains("sqlMessage")) {
            QString errorText = "Siirto epäonnistui: " + obj["sqlMessage"].toString();
            QMessageBox::warning(this, "Virhe", errorText);
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

/**
 * @brief Päivittää saldon näyttöön kun ikkuna avataan
 * @details getBalance() on jo kutsuttu Accountinfossta ennen ikkunan avaamista, mutta varmitetaan vielä showEventissä.
 */
void transfer::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->labelTransferAccountBalance->setText(balance + " €");
}
/**
 * @brief Takaisin nappi sulkee siirto ikkunan.
 */
void transfer::btnBackClicked()
{
    close();
}

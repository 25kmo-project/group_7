/**
 *@file Deposit.h
 *@brief Talletusdialogin toteutus.
 *@details Käsittelee nappien summan laskennan ja lähettää talletuksen API:lle.
 */
#include "deposit.h"
#include "ui_deposit.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

#include "apiclient.h"

Deposit::Deposit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Deposit)
{
    ui->setupUi(this);

    connect(ui->btnDepositBack, &QPushButton::clicked, this, &Deposit::btnDepositBackClicked);
    connect(ui->btnAddFive, &QPushButton::clicked, this, &Deposit::btnAddFiveClicked);
    connect(ui->btnAddTen, &QPushButton::clicked, this, &Deposit::btnAddTenClicked);
    connect(ui->btnAddTwenty, &QPushButton::clicked, this, &Deposit::btnAddTwentyClicked);
    connect(ui->btnAddFifty, &QPushButton::clicked, this, &Deposit::btnAddFiftyClicked);
    connect(ui->btnAddHundred, &QPushButton::clicked, this, &Deposit::btnAddHundredClicked);
    connect(ui->btnAddTwoHundred, &QPushButton::clicked, this, &Deposit::btnAddTwoHundredClicked);
    connect(ui->btnAddFiveHundred, &QPushButton::clicked, this, &Deposit::btnAddFiveHundredClicked);
    connect(ui->btnReset, &QPushButton::clicked, this, &Deposit::btnResetClicked);
    connect(ui->btnNewDeposit, &QPushButton::clicked, this, &Deposit::btnNewDepositClicked);
}

Deposit::~Deposit()
{
    delete ui;
}
/**
 * @brief Deposit::setToken
 * @param t JWT-token backend-kutsuille
 */
void Deposit::setToken(const QString &t)
{
    token = t; // vain UI-ikkunoita varten
}
/**
 * @brief Asettaa tilin ID:n, johon talletus kohdistuu
 * @param id Tilin-ID tietokannassa
 */

void Deposit::setAccountId(int id)
{
    accountId = id;
}
/**
 * @brief Lähettää talletuspyynnön backendille.
 * @param amount Talletettava summa euroina.
 * @note Käyttää ApiClient instance() singletonia, kutsu POST /bank_log/deposit
 */

void Deposit::btnDepositBackClicked()
{
    close();
}

// -----------------------------
//  YHTEINEN FUNKTIO TALLETUKSILLE
// -----------------------------
void Deposit::sendDepositRequest(int amount)
{
    QJsonObject json;
    json["my_account_id"] = accountId;
    json["amount"] = amount;

    auto reply = ApiClient::instance().post("bank_log/deposit", json);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        qDebug() << "DEPOSIT RESPONSE:" << response;

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Virhe talletuksessa", reply->errorString());
            reply->deleteLater();
            return;
        }

        QMessageBox::information(this, "Talletus onnistui", "Talletus suoritettu onnistuneesti.");
        emit depositSuccessful();

        currentAmount = 0;
        ui->labelAddedAmount->setText("0 €");

        reply->deleteLater();
    });
}
/**
 * @brief Käsittelee Talleta napin klikkauksen
 * @details Tarkistaa maksimirajan (1000euroa) ja että on suuurempi kuin 0 (>0)
 */

// -----------------------------
//  NAPIT LISÄÄ SUMMAA
// -----------------------------
void Deposit::btnAddFiveClicked()
{
    currentAmount += 5;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddTenClicked()
{
    currentAmount += 10;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddTwentyClicked()
{
    currentAmount += 20;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddFiftyClicked()
{
    currentAmount += 50;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddHundredClicked()
{
    currentAmount += 100;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddTwoHundredClicked()
{
    currentAmount += 200;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddFiveHundredClicked()
{
    currentAmount += 500;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

// -----------------------------
//  TALLETUS
// -----------------------------
void Deposit::btnNewDepositClicked()
{
    if (currentAmount > 10000) {
        currentAmount = 0;
        ui->labelAddedAmount->setText("0 €");
        QMessageBox::warning(this, "Virhe!", "Maksimi talletus on 10000 €");
        return;
    }

    if (currentAmount <= 0) {
        QMessageBox::warning(this, "Virhe!", "Syötä summa");
        return;
    }

    sendDepositRequest(currentAmount);
}

// -----------------------------
//  RESET
// -----------------------------
void Deposit::btnResetClicked()
{
    currentAmount = 0;
    ui->labelAddedAmount->setText("0 €");
}

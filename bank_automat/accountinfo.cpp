/**
 * @file accountinfo.cpp
 * @brief Tilin pääikkuna.
 * @details Näytetään tilin tiedot. Napit talletukseen, nostoon, siirtoon, tilitapahtumiin ja omiin tietoihin. Päivittää saldon automaattisesti toiminnon jälkeen.
 */

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
#include "transactions.h"


Accountinfo::Accountinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Accountinfo)
{
    ui->setupUi(this);

    connect(ui->Omatiedot, &QPushButton::clicked, this, &Accountinfo::btnMyDataClicked);
    connect(ui->btnWithdraw, &QPushButton::clicked, this, &Accountinfo::btnWithdrawClicked);
    connect(ui->btnDeposit, &QPushButton::clicked, this, &Accountinfo::btnNewDepositClicked);
    connect(ui->btnTransfer, &QPushButton::clicked, this, &Accountinfo::btnTransferClicked);
    connect(ui->btnTransactions, &QPushButton::clicked, this, &Accountinfo::btnTransactionsClicked);
}


Accountinfo::~Accountinfo()
{
    delete ui;
}

/**
 * @brief Asettaa tilin tyypin.
 */
void Accountinfo::setAccountType(const QString &type)
{
    accountType = type;
}

/**
 * @brief Asettaa käyttäjän ID:N
 */
void Accountinfo::setUsername(const QString &newUsername)
{
    username = newUsername;
}

/**
 * @brief Asettaa JWT-toknein
 */
void Accountinfo::setToken(const QByteArray &newToken)
{
    token = newToken;
}

/**
 * @brief Asettaa tilin ID:n (käytetään mm. refreshBalance() ja Transactions).
 */
void Accountinfo::setAccountId(int id)
{
    accountId = id;
}
/**
 * @brief Asettaa tilin tiedot UI:hin backendin vastauksesta.
 * @param obj QJsonObject /bank_account/{user}/{type} -kutsusta.
 */
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

/**
 * @brief Kutsutaan aina kun ikkuna avataan.
 * @details Hakee tuoreimmat tilin tiedot backendistä ja päivittää UI:n.
 */
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
/**
 * @brief "Omat tiedot" -nappi → avaa henkilötietonäkymän.
 */
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

            // Jos käyttäjä kirjautuu ulos Data-ikkunasta, suljetaan myös Accountinfo
            connect(objData, &Data::logoutRequested, this, &Accountinfo::close);

            objData->show();
        }

        reply->deleteLater();
    });
}
/**
 * @brief "Nosto" -nappi → avaa Withdraw-dialogin.
 */
void Accountinfo::btnWithdrawClicked()
{
    Withdraw *objWd = new Withdraw(this);
    objWd->token = token;
    objWd->accountId = accountId;
    objWd->balance = ui->labelBalance->text();

    connect(objWd, &Withdraw::withdrawDone, this, &Accountinfo::refreshBalance);
    objWd->show();
}
/**
 * @brief "Talletus" -nappi → avaa Deposit-dialogin.
 */
void Accountinfo::btnNewDepositClicked()
{
    Deposit *objDeposit = new Deposit(this);
    objDeposit->setToken(QString(token));
    objDeposit->setAccountId(accountId);

    connect(objDeposit, &Deposit::depositSuccessful, this, &Accountinfo::refreshBalance);
    objDeposit->show();
}
/**
 * @brief "Siirto" -nappi → avaa Transfer-dialogin.
 */
void Accountinfo::btnTransferClicked()
{
    transfer *objTransfer = new transfer(this);
    objTransfer->setToken(QString(token));
    objTransfer->setAccountId(accountId);
    objTransfer->balance = ui->labelBalance->text();

    connect(objTransfer, &transfer::transferSuccesful, this, &Accountinfo::refreshBalance);
    objTransfer->show();
}
/**
 * @brief Päivittää tilin saldon näytölle.
 * @details Kutsutaan automaattisesti noston, talletuksen tai siirron jälkeen.
 */
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
/**
 * @brief Takaisin-nappi (yleensä Qt Designerin automaattinen slot).
 * @details Lähettää signaalin MainWindow:lle ja sulkee ikkunan.
 */
void Accountinfo::on_btnBack_clicked()
{
    emit backRequested();
    close();
}
/**
 * @brief Kirjaudu ulos -nappi.
 * @details Palauttaa käyttäjän kirjautumisnäkymään.
 */
void Accountinfo::on_btnLogout_clicked()
{
    emit backRequested();
    close();
}

void Accountinfo::btnTransactionsClicked()
{
    Transactions *objTransactions = new Transactions(this);
    objTransactions->setToken(QString(token));
    objTransactions->setAccountId(accountId);
    objTransactions->updateLogs();
    objTransactions->show();
}

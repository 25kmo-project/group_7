#include "deposit.h"
#include "ui_deposit.h"
#include <QMessageBox>


void Deposit::setToken(const QString &t)
{
    token = t;
}

void Deposit::setAccountId(int id)
{
    accountId = id;

}

Deposit::Deposit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Deposit)
{
    ui->setupUi(this);
    manager=new QNetworkAccessManager (this);
    connect(manager, &QNetworkAccessManager::finished, this, &Deposit::onDepositReply);
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

void Deposit::btnDepositBackClicked()
{
    this->close();
}

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
    currentAmount +=50;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddHundredClicked()
{
    currentAmount +=100;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddTwoHundredClicked()
{
    currentAmount +=200;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddFiveHundredClicked()
{
    currentAmount +=500;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnNewDepositClicked()
{
    if (currentAmount > 10000){
        currentAmount = 0;
        ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
        QMessageBox::warning(this, "Virhe!", "Maksimi talletus on 10000 €");
        return;
    }else if (currentAmount <= 0) {
        QMessageBox::warning(this, "Virhe!", "Syötä summa");
    }else{
        QNetworkRequest req(Environment::base_url() + "bank_log/deposit");
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());

        QJsonObject json;
        json["my_account_id"] = accountId;
        json["amount"] = currentAmount;

        qDebug() << "TOKEN:" << token;
        qDebug() << "ACCOUNT ID:" << accountId;
        qDebug() << "URL:" << Environment::base_url() + "bank_log/deposit";
        qDebug() << "JSON:" << QJsonDocument(json).toJson();

        manager->post(req, QJsonDocument(json).toJson());

        qDebug() << "Tallennetaa: " << currentAmount << "€";
    }
}

void Deposit::btnResetClicked()
{
    currentAmount = 0;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::onDepositReply(QNetworkReply *reply)
{
    // qDebug() << "HTTP STATUS:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // qDebug() << "REPLY ERROR:" << reply->error();
    // qDebug() << "REPLY TEXT:" << reply->readAll();

    if(token.isEmpty() || accountId < 0) {
        QMessageBox::warning(this, "Virhe", "Tili tai token puuttuu!");
        return;
    }

    if(reply->error() == QNetworkReply::NoError) {

        QMessageBox::information(this, "Talletus onnistui", "Talletus suoritettu onnistuneesti.");
        ui->labelAddedAmount->setText(QString::number(currentAmount) + " € talletettu");
        currentAmount = 0;
        emit depositSuccessful();
    }else{
        QMessageBox::warning(this, "Virhe", reply->errorString());
    }
    reply->deleteLater();
}

#include "choosecard.h"
#include "environment.h"
#include "ui_choosecard.h"


#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

ChooseCard::ChooseCard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChooseCard)
{
    manager = new QNetworkAccessManager(this);
    ui->setupUi(this);
    connect(ui->btnDEBIT,&QPushButton::clicked,(this),&ChooseCard::btnDEBITClicked);
    connect(ui->btnCREDIT,&QPushButton::clicked,(this),&ChooseCard::btnCREDITClicked);

}

ChooseCard::~ChooseCard()
{
    delete ui;
}

void ChooseCard::btnDEBITClicked()
{

    QString url=Environment::base_url()+"bank_account/+2";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QByteArray myToken="Bearer "+token;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    reply = manager->get(request);



    connect(reply, &QNetworkReply::finished,this,&ChooseCard::ChooseCardSlot);


}

void ChooseCard::ChooseCardSlot()
{
    QByteArray response = reply->readAll();
    qDebug()<<response;
    reply->deleteLater();


}

void ChooseCard::btnCREDITClicked()
{
    QString url=Environment::base_url()+"bank_account/+3";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QByteArray myToken="Bearer "+token;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished,this,&ChooseCard::ChooseCardSlot);
}

/*void ChooseCard::on_btn_Credit_clicked()
{
    //cardSelected("credit");
    qDebug() << "Credit painettu";
    qDebug() << "Debit painettu";
    QString url = Environment::base_url() + "bank_account";
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + token);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &ChooseCard::debitTiedot);
    manager->get(request);

}


void ChooseCard::on_btn_Debit_clicked()
{
    //cardSelected("Debit");
    qDebug() << "Debit painettu";
    QString url = Environment::base_url() + "bank_account";
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + token);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
    this, &ChooseCard::debitTiedot);
    manager->get(request);

}

void ChooseCard::debitTiedot(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    qDebug() <<"ID: " << obj["account_id"].toInt();
    qDebug() << "Type:" << obj["account_type"].toString();
    qDebug() << "Number:" << obj["account_number"].toString();
    qDebug() << "Balance:" << obj["balance"].toString();
    qDebug() << "Credit limit:" << obj["credit_limit"].toString();
    qDebug() << "User ID:" << obj["user_id"].toInt();



    //ui->textDebug->setText(debug);

    reply->deleteLater();
}

void ChooseCard::creditTiedot(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    qDebug() <<"ID: " << obj["account_id"].toInt();
    qDebug() << "Type:" << obj["account_type"].toString();
    qDebug() << "Number:" << obj["account_number"].toString();
    qDebug() << "Balance:" << obj["balance"].toString();
    qDebug() << "Credit limit:" << obj["credit_limit"].toString();
    qDebug() << "User ID:" << obj["user_id"].toInt();

}

*/

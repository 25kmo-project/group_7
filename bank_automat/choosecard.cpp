#include "choosecard.h"
#include "environment.h"
#include "ui_choosecard.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "accountinfo.h"

ChooseCard::ChooseCard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChooseCard)
{
    manager = new QNetworkAccessManager(this); //HTTP_pyyntö varte
    ui->setupUi(this);
    connect(ui->btnDEBIT, &QPushButton::clicked, this, &ChooseCard::btnDEBITClicked);
    connect(ui->btnCREDIT, &QPushButton::clicked, this, &ChooseCard::btnCREDITClicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &ChooseCard::btnBackClicked);
}

ChooseCard::~ChooseCard()
{
    delete ui;
}

void ChooseCard::btnDEBITClicked()
{
    if (username.isEmpty() || token.isEmpty()) { //Varmistetaan että käyttäjän id ja token asetettu
        qDebug() << "ChooseCard: Username or token is empty! Cannot fetch accounts.";
        return;
    }

    QString url = Environment::base_url() + "bank_account/" + username + "/debit"; //backend url credille
    qDebug() << "ChooseCard: Fetching debit accounts from URL:" << url;

    QNetworkRequest request(url); //GET pyyntöjä
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray myToken = "Bearer " + token;
    request.setRawHeader(QByteArray("Authorization"), myToken);
    reply = manager->get(request); //Lähetetään pyyntö
    connect(reply, &QNetworkReply::finished, this, &ChooseCard::handleDebit); //Kun vastaus tulee nii handlecredit
    connect(reply, &QNetworkReply::errorOccurred, this, &ChooseCard::handleNetworkError);
}

void ChooseCard::btnCREDITClicked()
{
    if (username.isEmpty() || token.isEmpty()) { //Varmistetaan että käyttäjän id ja token asetettu
        qDebug() << "ChooseCard: Username or token is empty! Cannot fetch accounts.";
        return;
    }

    QString url = Environment::base_url() + "bank_account/" + username + "/credit"; //backend url debitille
    qDebug() << "ChooseCard: Fetching credit accounts from URL:" << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray myToken = "Bearer " + token;
    request.setRawHeader(QByteArray("Authorization"), myToken);
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ChooseCard::handleCredit); //Kun vastaus tulee nii handlecredit
    connect(reply, &QNetworkReply::errorOccurred, this, &ChooseCard::handleNetworkError);
}

void ChooseCard::handleDebit()
{

    QByteArray response = reply->readAll(); //luetaan backendin json
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject obj = doc.object();
    openAccount(obj); //Avataan data annetulla json datalla
}
    /*qDebug() << "handleDebit() called";
    QByteArray response = reply->readAll();
    qDebug() << "Raw response:" << response;
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull()) {
        qDebug() << "ChooseCard: Invalid JSON in debit response";
        return;
    }

    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        qDebug() << "Single account object found:" << obj;
        if (obj["account_type"].toString() == "debit" && obj["user_id"].toInt() == username.toInt()) {
            qDebug() << "DEBIT selected (single object):" << obj;
            openAccount(obj);
            return;
        }
    } else if (doc.isArray()) {
        QJsonArray arr = doc.array();
        for (auto it = arr.constBegin(); it != arr.constEnd(); ++it) {
            QJsonObject obj = it->toObject();
            qDebug() << "Checking account in array:" << obj;
            if (obj["account_type"].toString() == "debit" && obj["user_id"].toInt() == username.toInt()) {
                qDebug() << "DEBIT selected (from array):" << obj;
                openAccount(obj);
                return;
            }
        }
    }

    qDebug() << "Debit-tiliä ei löytynyt!";
}*/

void ChooseCard::handleCredit()
{

    QByteArray response = reply->readAll(); //luetaan backendin json
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject obj = doc.object();
    openAccount(obj); //Avataan data annetulla json datalla
}
    /*qDebug() << "handleCredit() called";
    QByteArray response = reply->readAll();
    qDebug() << "Raw response:" << response;
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull()) {
        qDebug() << "ChooseCard: Invalid JSON in credit response";
        return;
    }


    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        qDebug() << "Single account object found:" << obj;
        if (obj["account_type"].toString() == "credit" && obj["user_id"].toInt() == username.toInt()) {
            qDebug() << "CREDIT selected (single object):" << obj;
            openAccount(obj);
            return;
        }
    } else if (doc.isArray()) {
        QJsonArray arr = doc.array();

        for (auto it = arr.constBegin(); it != arr.constEnd(); ++it) {
            QJsonObject obj = it->toObject();
            qDebug() << "Checking account in array:" << obj;
            if (obj["account_type"].toString() == "credit" && obj["user_id"].toInt() == username.toInt()) {
                qDebug() << "CREDIT selected (from array):" << obj;
                openAccount(obj);
                return;
            }
        }
    }

    qDebug() << "Credit-tiliä ei löytynyt!";
}*/

void ChooseCard::openAccount(const QJsonObject &obj)
{
    //emit cardSelected(obj["account_type"].toString());

    // Luodaan Accountinfo-ikkuna ja annetaan sille kaikki tarvittava data
    Accountinfo *acc = new Accountinfo(this);
    acc->setAccountData(obj); // JSON-tilidata
    acc->setToken(token); // Token API-kutsuja varten
    acc->setUsername(username); // Käyttäjän ID
    acc->setAccountId(obj["account_id"].toInt());
    acc->setAccountType(obj["account_type"].toString());
    acc->show();
    this->close(); // Suljetaan ChooseCard → siirrytään Accountinfoon
}

void ChooseCard::setChooseCard(const QByteArray &newChooseCard)
{
    token = newChooseCard;   // Tallennetaan token myöhempiä API-kutsuja varten
    qDebug() << "ChooseCard: Token set to" << token;
}

void ChooseCard::setUsername(const QString &newUsername)
{
    username = newUsername; // Tallennetaan käyttäjän ID
    qDebug() << "ChooseCard: Username set to" << username;
}

void ChooseCard::handleNetworkError(QNetworkReply::NetworkError error)
{
    qDebug() << "ChooseCard: Network error:" << error << "-" << reply->errorString();
}

void ChooseCard::btnBackClicked()
{
    emit backRequested();
    qDebug() << "ChooseCard: Takaisin painettu → suljetaan dialogi";
    this->close();
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
    //manager->get(request);

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

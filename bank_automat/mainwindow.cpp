#include "choosecard.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox> //Messageboxia varten
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::btnLoginSlot);

    manager = new QNetworkAccessManager(this);//Luodaan verkko.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnLoginSlot()
{
    QString url = Environment::base_url() + "bank_kirjautuminen"; //Backendin kirjautusmis url
    QNetworkRequest request(url); // Post pyynnnöt
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jObject; //JSon dataa
    jObject.insert("card_number", ui->textUsername->text());
    jObject.insert("pin", ui->textPassword->text());
    QJsonDocument jsonDoc(jObject); //Lähetään dataa backendil
    reply = manager->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &MainWindow::loginAction); //kirjautuu
}

void MainWindow::loginAction()
{
    QByteArray responseData = reply->readAll(); //Backendin vastaus
    qDebug() << "Response data:" << responseData;

    if (responseData == "-4078" || responseData.length() == 0) { //Tietokanta virhe
        ui->labelInfo->setText("Virhe tietokantayhteydessä");
    } else {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData); //Jos onnistuu nii tokenin palautus
        QJsonObject jsonObject = jsonDoc.object();
        if (jsonObject.contains("token")) {
            qDebug() << "User ID at login:" << ui->textUsername->text();

            // Siirrä token-määrittely tänne ylös, ennen ChooseCard:ia
            QString token = jsonObject["token"].toString();
            QByteArray tokenBytes = token.toUtf8();
            int userId = jsonObject["user_id"].toInt();
            QString cardType = jsonObject["card_type"].toString();

            qDebug() << "Login OK. User ID:" << userId << "Card type:" << cardType;
            qDebug() << "Login ok";
            qDebug() << "Token:" << token;

            if (cardType == "credit") {
                qDebug() << "Kirjaudutaan suoraan DEBIT-tiliin";
                //Haetaan credit-tilin tiedot

                QString url = Environment::base_url() + "bank_account/" + QString::number(userId) + "/credit";

                QNetworkRequest request(url); //Lähettää GET
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                request.setRawHeader("Authorization", "Bearer " + tokenBytes);

                QNetworkReply *accReply = manager->get(request);
                //Avaa accountinfon

                connect(accReply, &QNetworkReply::finished, this, [=]() {
                    QByteArray accData = accReply->readAll();
                    QJsonDocument accDoc = QJsonDocument::fromJson(accData);
                    QJsonObject obj = accDoc.object();

                    Accountinfo *acc = new Accountinfo(this);//Luodaan accouninfo ja annetaan JSON-data
                    acc->setToken(tokenBytes);
                    acc->setUsername(QString::number(userId));
                    acc->setAccountData(obj);
                    acc->show();

                    this->hide(); //Piilotetaan kirjautumisikkuna
                    accReply->deleteLater();
                });

                reply->deleteLater();
                return;
            }


            if (cardType == "debit") {
                qDebug() << "Kirjaudutaan suoraan DEBIT-tiliin";
                //Haetaan debit tilin tiedot

                QString url = Environment::base_url() + "bank_account/" + QString::number(userId) + "/debit";

                QNetworkRequest request(url);
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                request.setRawHeader("Authorization", "Bearer " + tokenBytes);

                QNetworkReply *accReply = manager->get(request);
                //avataan accouninfo

                connect(accReply, &QNetworkReply::finished, this, [=]() {
                    QByteArray accData = accReply->readAll();
                    QJsonDocument accDoc = QJsonDocument::fromJson(accData);
                    QJsonObject obj = accDoc.object();

                    Accountinfo *acc = new Accountinfo(this);
                    acc->setToken(tokenBytes);
                    acc->setUsername(QString::number(userId));
                    acc->setAccountData(obj);
                    acc->show();

                    this->hide();
                    accReply->deleteLater();
                });

                reply->deleteLater();
                return;
            }






            if (cardType == "dual") {
                qDebug() << "Kirjaudutaan DUAL-tilillä → avataan ChooseCard";

                ChooseCard *objchoose = new ChooseCard(this);
                objchoose->setUsername(QString::number(userId));
                objchoose->setChooseCard(tokenBytes);

                // Kun käyttäjä valitsee debit/credit, ChooseCard avaa Accountinfo-ikkunan
                connect(objchoose, &ChooseCard::cardSelected, this, &MainWindow::onCardSelected);

                objchoose->exec();   // Näyttää valintadialogin
                reply->deleteLater();
                return;
            }



            /*ChooseCard *objchoose = new ChooseCard(this);
            objchoose->setUsername(QString::number(userId));
            objchoose->setChooseCard(tokenBytes);  // Nyt tokenBytes on määritelty!
            connect(objchoose, &ChooseCard::cardSelected, this, &MainWindow::onCardSelected);
            objchoose->exec();*/

            //Accountinfo *objAccountinfo = new Accountinfo(this);
            ///objAccountinfo->setToken(tokenBytes);
            //objAccountinfo->setUsername(ui->textUsername->text());
           // objAccountinfo->show();
        } else {
            ui->labelInfo->setText("Tunnus ja salasana eivät täsmää");
            QMessageBox::warning (this, "Kirjautumisvirhe","Tunnus ja salasana eivät täsmää. Yritä uudelleen.");
            ui->textUsername->clear();
            ui->textPassword->clear();
            ui->textUsername->setFocus();
        }
    }
    reply->deleteLater();
}

void MainWindow::onCardSelected(QString type)
{
    qDebug() << "Kortti valittu:" << type;

}

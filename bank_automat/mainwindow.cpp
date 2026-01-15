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
    manager = new QNetworkAccessManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnLoginSlot()
{
    QString url = Environment::base_url() + "bank_kirjautuminen";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jObject;
    jObject.insert("user_id", ui->textUsername->text());
    jObject.insert("pin", ui->textPassword->text());
    QJsonDocument jsonDoc(jObject);
    reply = manager->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &MainWindow::loginAction);
}

void MainWindow::loginAction()
{
    QByteArray responseData = reply->readAll();
    qDebug() << "Response data:" << responseData;

    if (responseData == "-4078" || responseData.length() == 0) {
        ui->labelInfo->setText("Virhe tietokantayhteydessä");
    } else {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonDoc.object();
        if (jsonObject.contains("token")) {
            qDebug() << "User ID at login:" << ui->textUsername->text();

            // Siirrä token-määrittely tänne ylös, ennen ChooseCard:ia
            QString token = jsonObject["token"].toString();
            QByteArray tokenBytes = token.toUtf8();
            qDebug() << "Login ok";
            qDebug() << "Token:" << token;

            ChooseCard *objchoose = new ChooseCard(this);
            objchoose->setUsername(ui->textUsername->text());
            objchoose->setChooseCard(tokenBytes);  // Nyt tokenBytes on määritelty!
            connect(objchoose, &ChooseCard::cardSelected, this, &MainWindow::onCardSelected);
            objchoose->exec();

            //Accountinfo *objAccountinfo = new Accountinfo(this);
            //objAccountinfo->setToken(tokenBytes);
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

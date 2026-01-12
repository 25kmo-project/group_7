#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::btnLoginSlot);
    manager=new QNetworkAccessManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnLoginSlot()
{
    QString url=Environment::base_url()+"bank_kirjautuminen";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jObject;
    jObject.insert("user_id", ui->textUsername->text());
    jObject.insert("pin",ui->textPassword->text());
    QJsonDocument jsonDoc(jObject);
    reply=manager->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &MainWindow::loginAction);


}

void MainWindow::loginAction()
{
    QByteArray responseData=reply->readAll();
    if(responseData == "-4078" || responseData.length() == 0) {
        ui->labelInfo->setText("Virhe tietokantayhteydessä");
    }
    else {
        QJsonDocument jsonDoc=QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject=jsonDoc.object();
        if(jsonObject.contains("token")){
            QString token=jsonObject["token"].toString();
            // qDebug()<<"Login ok";
            qDebug()<<token;
        }
        else {
            ui->labelInfo->setText("Tunnus ja salasana eivät täsmää");
        }
    }
    // qDebug()<<responseData;
}

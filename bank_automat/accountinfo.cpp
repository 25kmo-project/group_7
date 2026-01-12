#include "accountinfo.h"
#include "ui_accountinfo.h"

Accountinfo::Accountinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Accountinfo)
{
    ui->setupUi(this);
    manager=new QNetworkAccessManager(this);
    connect(ui->btnMyData, &QPushButton::clicked,this,&Accountinfo::btnMyDataClicked);
}

Accountinfo::~Accountinfo()
{
    delete ui;
}

void Accountinfo::setUsername(const QString &newUsername)
{
    username = newUsername;
    ui->labelUsername->setText(username);
}

void Accountinfo::setToken(const QByteArray &newToken)
{
    token = newToken;
    qDebug()<<token;
}

void Accountinfo::btnMyDataClicked()
{
    QString url=Environment::base_url()+"bank_user/"+username;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QByteArray myToken="Bearer "+token;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    reply=manager->get(request);
    connect(reply, &QNetworkReply::finished,this,&Accountinfo::MyDataSlot);



}

void Accountinfo::MyDataSlot()
{
    QByteArray response=reply->readAll();
    qDebug()<<response;

}

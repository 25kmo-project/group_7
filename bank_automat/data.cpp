#include "data.h"
#include "ui_data.h"

Data::Data(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Data)
{
    ui->setupUi(this);
}

Data::~Data()
{
    delete ui;
}

void Data::setTestData(const QByteArray &newTestData)
{
    testData = newTestData;
    QJsonDocument jsonDoc=QJsonDocument::fromJson(testData);
    QJsonObject jsonObject=jsonDoc.object();

    ui->labelfname->setText(jsonObject["first_name"].toString());
    ui->labelLastName->setText(jsonObject["last_name"].toString());
    ui->labelAddress1->setText(jsonObject["address"].toString());

}

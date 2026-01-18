#include "data.h"
#include "ui_data.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>  // Lisätty debuggausta varten

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
    QJsonDocument jsonDoc = QJsonDocument::fromJson(testData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Data: Invalid JSON in setTestData:" << testData;
        // Aseta fallbackit, jos parsinta epäonnistuu
        ui->labelFirstName->setText("Ei dataa");
        ui->labelLastName->setText("Ei dataa");
        ui->labelAddress->setText("Ei dataa");
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    qDebug() << "Data: Parsed object in setTestData:" << jsonObject;  // Debuggaa objekti

    // Aseta labelit fallbackilla (jos avain puuttuu)
    ui->labelFirstName->setText(jsonObject.value("first_name").toString("Ei dataa"));
    qDebug() << "labelFirstName set to:" << ui->labelFirstName->text();

    ui->labelLastName->setText(jsonObject.value("last_name").toString("Ei dataa"));
    qDebug() << "labelLastName set to:" << ui->labelLastName->text();

    ui->labelAddress->setText(jsonObject.value("address").toString("Ei dataa"));
    qDebug() << "labelAddress set to:" << ui->labelAddress->text();

    this->update();  // Pakota UI päivittyä heti
}

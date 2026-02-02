#include "data.h"
#include "ui_data.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QApplication>
#include "mainwindow.h"


Data::Data(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Data)
{
    ui->setupUi(this);

    // Takaisin-nappi → sulkee ikkunan
    connect(ui->btnBack, &QPushButton::clicked, this, &Data::btnBackClicked);

    // Kirjaudu ulos -nappi → sulkee ikkunan ja palaa login-näkymään
    connect(ui->btnLogOut, &QPushButton::clicked, this, &Data::btnLogOutClicked);
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

        // Fallback-arvot virhetilanteessa
        ui->labelFirstName->setText("Ei dataa");
        ui->labelLastName->setText("Ei dataa");
        ui->labelAddress->setText("Ei dataa");
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    qDebug() << "Data: Parsed object in setTestData:" << jsonObject;

    // Asetetaan UI-arvot fallbackilla, jos avain puuttuu
    ui->labelFirstName->setText(jsonObject.value("first_name").toString("Ei dataa"));
    ui->labelLastName->setText(jsonObject.value("last_name").toString("Ei dataa"));
    ui->labelAddress->setText(jsonObject.value("address").toString("Ei dataa"));

    this->update(); // Päivitetään UI välittömästi
}


void Data::btnBackClicked()
{
    this->close();
    qDebug() << "Data: Takaisin nappi painettu - ikkuna suljettu";
}


void Data::btnLogOutClicked()
{
    qDebug() << "Data: Kirjaudu-ulos nappi painettu.";

    this->close(); // Suljetaan Data-ikkuna ensin

    emit logoutRequested(); // Ilmoitetaan Accountinfo-ikkunalle

    // Avataan uusi kirjautumisikkuna
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}

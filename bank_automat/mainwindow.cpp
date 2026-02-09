#include "choosecard.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apiclient.h"
#include "environment.h"

#include <QMessageBox>
#include <QEvent>
#include <QApplication>
#include <QNetworkReply>
#include <QLineEdit>
#include <qevent.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inaktiivisuusajastin
    inactivityTimer = new QTimer(this);
    inactivityTimer->setInterval(30000);
    // 10 sek inaktiivisuusajastin
    loginInactivityTimer = new QTimer(this);
    loginInactivityTimer->setInterval(10000);
    //Login-napin signaali
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::btnLoginSlot);
    // Kirjautuminen Enterillä
    connect(ui->textPassword, &QLineEdit::returnPressed, this, &MainWindow::btnLoginSlot);
    // Ajastimen timeout-> Käyttäjä ollut liian kauan tekemättä mitäöän
    connect(inactivityTimer, &QTimer::timeout, this, &MainWindow::onInactivityTimeout);
    connect(loginInactivityTimer, &QTimer::timeout, this, &MainWindow::onLoginInactivityTimeout);
    //Kuuntelee hiireä ja näppäimistö
    qApp->installEventFilter(this);
    inactivityTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Nollataan inaktiivisuusajastimet aina kun käyttäjä liikuttaa hiirtä tai käyttää näppäimistöä
    if (event->type() == QEvent::MouseMove ||
        event->type() == QEvent::KeyPress ||
        event->type() == QEvent::MouseButtonPress)
    {
        inactivityTimer->start();

        // Login-inaktiivisuusajastin käynnistyy jos kentissä on tekstiä
        if (ui && (!ui->textUsername->text().isEmpty() || !ui->textPassword->text().isEmpty())) {
            loginInactivityTimer->start();
        } else {
        // Jos kentät ovat tyhjiä, pysäytetään login-inaktiivisuusajastin
            loginInactivityTimer->stop();

        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onInactivityTimeout()
{
    qDebug() << "30s Inaktiivisuus: palautetaan alkutilaan";

    // suljetaan ajastimet ja eventfilter kutsu poies päältä
    qApp->removeEventFilter(this);
    inactivityTimer->stop();
    loginInactivityTimer->stop();

    // suljetaan muut ikkunat
    for (QWidget *w : QApplication::topLevelWidgets()) { if (w != this) w->close(); }

    this->show();
    this->raise();

    qApp->installEventFilter(this);
    inactivityTimer->start();
}

void MainWindow::onLoginInactivityTimeout()
{
    qDebug() << "10s Inaktiivisuus -> Tyhjennetään kirjautumiskentät.";
    if (ui) {
        ui->textPassword->clear();
        ui->textUsername->clear();
    }
    loginInactivityTimer->stop();
}

void MainWindow::btnLoginSlot()
{
    QString endpoint = "bank_kirjautuminen";
    // JSON-data kirjautumiseen
    QJsonObject body;
    body["card_number"] = ui->textUsername->text();
    body["pin"] = ui->textPassword->text();
    // Lähetetään POST backendille
    QNetworkReply *reply = ApiClient::instance().post(endpoint, body);
    // Vastaus saapuu-> käsitellään login funktiossa
    connect(reply, &QNetworkReply::finished, this, [=]() {
        loginAction(reply);
    });
}

void MainWindow::loginAction(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();
    qDebug() << "Response data:" << responseData;
    //Palauttaa 4078 jos tietokantayhteys epäonnistuu
    if (responseData == "-4078" || responseData.isEmpty()) {
        ui->LabelErrorMessage->setText("Virhe tietokantayhteydessä");
        reply->deleteLater();
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonDoc.object();
    //Jos token puuttuu -> kirjautuminen epäonnistuu
    if (!jsonObject.contains("token")) {
        QString backendMessage = jsonObject.contains("message")
        ? jsonObject["message"].toString()
        : "Sori, tapahtu virhe";

        ui->LabelErrorMessage->setText("Tunnus ja salasana eivät täsmää");
        QMessageBox::warning(this, "Kirjautumisvirhe", backendMessage);
        //tyhjentää kentät uutta yritystä varten
        ui->textUsername->clear();
        ui->textPassword->clear();
        ui->textUsername->setFocus();

        reply->deleteLater();
        return;
    }

    // --- Kirjautuminen OK ---
    QString token = jsonObject["token"].toString();
    int userId = jsonObject["user_id"].toInt();
    QString cardType = jsonObject["card_type"].toString();
    // Tämä tallentaa tokenin ApiClienttiin
    ApiClient::instance().setToken(token.toUtf8());

    qDebug() << "Login OK. User ID:" << userId << "Card type:" << cardType;
    qDebug() << "Token:" << token;

    // Tyhjennetään kirjautumiskentät onnistuneen kirjautumisen yhteydessä
    if (ui) {
        ui->textUsername->clear();
        ui->textPassword->clear();
        ui->LabelErrorMessage->clear();
    }

    // Pysäytetään login-inaktiivisuusajastin, koska kentät ovat nyt tyhjiä
    if (loginInactivityTimer->isActive()) {
        loginInactivityTimer->stop();
    }

    // --- Debit / Credit / Dual käsittely ---
    if (cardType == "credit") {
        openAccountWindow(userId, "credit");
    }
    else if (cardType == "debit") {
        openAccountWindow(userId, "debit");
    }
    else if (cardType == "dual") {
        // dual-kortti -> käyttäjä pitää valita debit tai credit
        ChooseCard *dlg = new ChooseCard(this);
        dlg->setUsername(QString::number(userId));
        dlg->setChooseCard(token.toUtf8());

        connect(dlg, &ChooseCard::cardSelected, this, &MainWindow::onCardSelected);
        connect(dlg, &ChooseCard::backRequested, this, &MainWindow::show);

        dlg->exec();
    }

    reply->deleteLater();
}

void MainWindow::openAccountWindow(int userId, const QString &type)
{
    // Haetaan tilitiedot backendistä
    QString endpoint = "bank_account/" + QString::number(userId) + "/" + type;

    QNetworkReply *accReply = ApiClient::instance().get(endpoint);

    connect(accReply, &QNetworkReply::finished, this, [=]() {
        QByteArray accData = accReply->readAll();
        QJsonDocument accDoc = QJsonDocument::fromJson(accData);
        QJsonObject obj = accDoc.object();
        //Luodaan tilitietoikkuna
        Accountinfo *acc = new Accountinfo(this);
        connect(acc, &Accountinfo::backRequested, this, &MainWindow::show);

        acc->setToken(ApiClient::instance().getToken());
        acc->setUsername(QString::number(userId));
        acc->setAccountData(obj);

        acc->show();
        this->hide(); //Piilotetaanm login-ikkuna tilinäkymän ajaksi

        accReply->deleteLater();
    });
}

void MainWindow::onCardSelected(QString type)
{
    qDebug() << "Kortti valittu:" << type;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Suljetaan koko sovellus, jos pöäikkuna suljetaan
    QApplication::quit();
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}

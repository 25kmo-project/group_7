#include "mainwindow.h"
#include <QFile>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QApplication>
#include <QFileInfo>
#include <QTimer>

QString loadStyleSheet(const QString &path) { QFile file(path); if (!file.open(QFile::ReadOnly | QFile::Text)) { qDebug() << "Failed to load QSS:" << file.errorString(); return ""; } return QString::fromUtf8(file.readAll()); }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString basePath = QCoreApplication::applicationDirPath() + "/../..";

    QString qssPath = basePath + "/styles.qss";

    qDebug() << "Absolute path:" << QFileInfo(qssPath).absoluteFilePath();
    qDebug() << "Exists:" << QFile::exists(qssPath);

    qDebug() << QFile::exists(qssPath);


    a.setStyleSheet(loadStyleSheet(qssPath));
    qDebug() << "Initial QSS loaded";

<<<<<<< Updated upstream
    // QFileSystemWatcher *watcher = new QFileSystemWatcher(); // watcher->addPath(qssPath); // QObject::connect(watcher, &QFileSystemWatcher::fileChanged, [&](const QString &path){ // qDebug() << "WATCHER TRIGGERED"; // QTimer::singleShot(100, [&](){ // a.setStyleSheet(loadStyleSheet(qssPath)); // watcher->addPath(qssPath); // }); // });

=======
    /* Top bar - napit (sama tyyli molemmissa ikkunoissa) */
    QPushButton#btnBack,
    QPushButton#btnLogout,
    QPushButton#btnLogOut {
        background-color: #DC3545;
        color: white;
        font-size: 21pt;
        font-weight: bold;
        border-radius: 12px;
        padding: 15px 50px;
        min-width: 220px;
        min-height: 65px;
    }

    QPushButton#btnLogOut {
        min-width: 240px;   /* Kirjaudu ulos on vähän leveämpi */
    }

    /* Neljä päätoimintanappia (Talletus, Nosto...) */
    QPushButton#btnDeposit, QPushButton#btnWithdraw,
    QPushButton#btnTransfer, QPushButton#btnTransactions {
        background-color: #1E90FF;
        font-size: 20pt;
        font-weight: bold;
        border-radius: 16px;
        padding: 18px 30px;
        min-height: 82px;
    }

    /* Omat tiedot -nappi */
    QPushButton#Omatiedot {
        background-color: #007BFF;
        font-size: 19pt;
        font-weight: bold;
        border-radius: 14px;
        padding: 18px 50px;
        min-height: 68px;
        min-width: 210px;
    }

    /* Info-kortti (Tilitiedot) */
    QFrame#infoFrame {
        background-color: #001F3F;
        border: 2px solid #1E90FF;
        border-radius: 20px;
    }

    /* Omat tiedot -kortti */
    QFrame#profileCard {
        background-color: #001F3F;
        border: 3px solid #1E90FF;
        border-radius: 22px;
    }

    /* Otsikot */
    QLabel#labelTitle {
        font-size: 34pt;
        font-weight: bold;
    }

    /* Kaikki labelit (poistaa viivat ja tekee siistin) */
    QLabel {
        font-size: 21pt;
        border: none;
        border-bottom: none;
        padding: 8px 0;
    }

    /* Saldo */
    QLabel#labelBalance {
        font-size: 40pt;
        color: #00FF88;
        font-weight: bold;
    }

    /* Harmaat otsikkorivit */
    QLabel#labelIDTitle, QLabel#labelTypeTitle,
    QLabel#labelAccountNumberTitle, QLabel#labelBalanceTitle,
    QLabel#labelCreditLimitTitle {
        color: #BBBBBB;
    }

    /* Omat tiedot -arvot (Heikki, Meikäläinen jne.) */
    QLabel#labelFirstName,
    QLabel#labelLastName,
    QLabel#labelAddress {
        background-color: #112233;
        padding: 12px 20px;
        border-radius: 10px;
        font-size: 22pt;
        font-weight: 500;
    }
)");
>>>>>>> Stashed changes

    MainWindow w;
    w.show();
    return a.exec();
}

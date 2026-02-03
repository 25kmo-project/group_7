#include "mainwindow.h"
#include <QApplication>
#include <QDebug>


int main (int argc, char *argv[])
{
    QApplication a(argc, argv);


    a.setStyleSheet(R"(
    QWidget {
        background-color: #0A2540;
        color: #FFFFFF;
        font-family: "Segoe UI", Arial, sans-serif;
    }

    /* Top bar - napit */
    QPushButton#btnBack, QPushButton#btnLogout {
        background-color: #DC3545;
        font-size: 21pt;
        font-weight: bold;
        border-radius: 12px;
        padding: 15px 50px;
        min-width: 220px;
        min-height: 65px;
    }

    /* Neljä päätoimintanappia – nyt selvästi pienemmät */
    QPushButton#btnDeposit, QPushButton#btnWithdraw,
    QPushButton#btnTransfer, QPushButton#btnTransactions {
        background-color: #1E90FF;
        font-size: 20pt;           /* pienempi fontti */
        font-weight: bold;
        border-radius: 16px;
        padding: 18px 30px;
        min-height: 82px;          /* oli 110px → nyt 82px */
    }

    /* Omat tiedot -nappi – nyt isompi ja selkeä */
    QPushButton#Omatiedot {
        background-color: #007BFF;
        font-size: 19pt;
        font-weight: bold;
        border-radius: 14px;
        padding: 18px 50px;
        min-height: 68px;
        min-width: 210px;
    }

    /* Info-kortti – reunus ohuempi ja siisti */
    QFrame#infoFrame {
        background-color: #001F3F;
        border: 2px solid #1E90FF;   /* ohut sininen reunus */
        border-radius: 20px;
    }

    /* Poistetaan kaikki siniset viivat rivien välistä */
    QLabel {
        font-size: 21pt;
        border: none;                /* poistaa kaikki reunat */
        border-bottom: none;         /* ei viivoja rivien alle */
        padding: 6px 0;
    }

    // QFileSystemWatcher *watcher = new QFileSystemWatcher(); // watcher->addPath(qssPath); // QObject::connect(watcher, &QFileSystemWatcher::fileChanged, [&](const QString &path){ // qDebug() << "WATCHER TRIGGERED"; // QTimer::singleShot(100, [&](){ // a.setStyleSheet(loadStyleSheet(qssPath)); // watcher->addPath(qssPath); // }); // });

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
    /* Saldo vihreänä ja isona */
    QLabel#labelBalance {
        font-size: 40pt;
        color: #00FF88;
        font-weight: bold;
    }

    /* Otsikkorivit harmaammaksi */
    QLabel#labelIDTitle, QLabel#labelTypeTitle,
    QLabel#labelAccountNumberTitle, QLabel#labelBalanceTitle,
    QLabel#labelCreditLimitTitle {
        color: #BBBBBB;
    }
)");

    MainWindow w;
    w.show();
    return a.exec();
}

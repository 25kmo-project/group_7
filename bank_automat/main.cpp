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

    /* ───── Top bar - napit ───── */
    QPushButton#btnBack, QPushButton#btnLogout, QPushButton#btnLogOut,
    QPushButton#btnWithdrawBack, QPushButton#btnDepositBack,
    QPushButton#btnTransferBack, QPushButton#btnClose {
        background-color: #DC3545;
        font-size: 19pt;
        font-weight: bold;
        border-radius: 12px;
        padding: 12px 40px;
        min-width: 140px;
        min-height: 16px;
    }

    /* ───── Siniset toimintonapit ───── */
    QPushButton#btnDeposit, QPushButton#btnWithdraw, QPushButton#btnTransfer,
    QPushButton#btnTransactions, QPushButton#btnNosta, QPushButton#btnNewDeposit,
    QPushButton#btnAddFive, QPushButton#btnAddTen, QPushButton#btnAddTwenty,
    QPushButton#btnAddFifty, QPushButton#btnAddHundred, QPushButton#btnAddTwoHundred,
    QPushButton#btnAddFiveHundred, QPushButton#btn20, QPushButton#btn40,
    QPushButton#btn50, QPushButton#btn100, QPushButton#btnWithdrawBack_6,
    QPushButton#btnNewer, QPushButton#btnOlder {
        background-color: #1E90FF;
        font-size: 19pt;
        font-weight: bold;
        border-radius: 12px;
        padding: 12px 30px;
        min-width: 120px;
        min-height: 16px;
    }
    /* Omat tiedot -nappi – sama tyyli  */
    QPushButton#Omatiedot {
        background-color: #1E90FF;
        font-size: 19pt;
        font-weight: bold;
        border-radius: 12px;
        padding: 12px 40px;
        min-width: 180px;
        min-height: 55px;
    }

    QPushButton#Omatiedot:hover {
        background-color: #3AA0FF;
    }

    QPushButton#pushButton {
        background-color: #C8102E;
        color: white;
        font-size: 20pt;
        font-weight: bold;
        border: none;
        border-radius: 10px;
        padding: 14px 50px;
        min-width: 170px;
        min-height: 58px;
    }

    QPushButton#pushButton:hover {
        background-color: #E03C4E;
    }

    /* ───── Vihreät vahvistusnapit ───── */
    QPushButton#btnLogin, QPushButton#btnNosta, QPushButton#btnTransferMoney {
        background-color: #28A745;
        font-size: 22pt;
        font-weight: bold;
        border-radius: 14px;
        padding: 14px 60px;
        min-height: 16px;
        min-width: 220px;
    }

    QPushButton#btnReset {
        background-color: #DC3545;
        font-size: 19pt;
        font-weight: bold;
        border-radius: 12px;
        padding: 12px 40px;
        min-height: 16px;
    }

    /* ───── Syöttökentät ───── */
    QLineEdit#textUsername, QLineEdit#textPassword,
    QLineEdit#lineEditAmount,
    QLineEdit#textTransferAccountNumber, QLineEdit#textTrasferAmount {
        background-color: #FFFFFF;
        color: #000000;
        font-size: 20pt;
        padding: 8px 12px;
        border: 2px solid #1E90FF;
        border-radius: 8px;
        min-height: 16px;
    }

    /* ───── Kirjautumisikkuna ───── */
    QFrame#loginCard {
        background-color: #001F3F;
        border: 1px solid #1E90FF;
        border-radius: 24px;
        max-width: 520px;
    }

    QLabel#label_4 {                    /* Tervetuloa! */
        color: #FFFFFF;
        font-size: 36pt;
        font-weight: bold;
    }

    QLabel#LabelErrorMessage {
        font-size: 18pt;
        font-weight: bold;
        color: #FF4444;
    }

    /* ───── Muut labelit ja otsikot ───── */
    QLabel {
        font-size: 19pt;
    }

    QLabel#label, QLabel#label_2,
    QLabel#labelIDTitle, QLabel#labelTypeTitle,
    QLabel#labelAccountNumberTitle, QLabel#labelBalanceTitle,
    QLabel#labelCreditLimitTitle {
        color: #BBBBBB;
    }

    QLabel#labelBalance {
        font-size: 40pt;
        color: #00FF88;
        font-weight: bold;
    }

    /* Info-kortit */
    QFrame#infoFrame, QFrame#profileCard, QFrame#tableFrame {
        background-color: #001F3F;
        border: 3px solid #1E90FF;
        border-radius: 20px;
    }

    /* Tilitapahtumat-taulukko */
    QTableView#tableTransactions {
        background-color: #001F3F;
        color: #FFFFFF;
        font-size: 17pt;
        gridline-color: #334455;
        alternate-background-color: #112233;
        selection-background-color: #1E90FF;
    }

    QHeaderView::section {
        background-color: #1E90FF;
        color: white;
        font-weight: bold;
        padding: 8px;
    }
)");

    MainWindow w;
    w.show();
    return a.exec();
}

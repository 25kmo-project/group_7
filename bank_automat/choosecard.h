#ifndef CHOOSECARD_H
#define CHOOSECARD_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class ChooseCard;
}

class ChooseCard : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCard(QWidget *parent = nullptr);
    ~ChooseCard();
    void setUsername(const QString &newUsername);
    void setChooseCard(const QByteArray &newChooseCard);

signals:
    void cardSelected(QString type);  // Poistettu kommentti: Tämä signaali tarvitaan connect-funktiolle MainWindow:ssa

private slots:
    //void on_btn_Credit_clicked();  // Jos tarvitset, poista kommentti ja toteuta .cpp:ssä
    //void on_btn_Debit_clicked();   // Jos tarvitset, poista kommentti ja toteuta .cpp:ssä
    //void debitTiedot(QNetworkReply *reply);  // Jos tarvitset, poista kommentti
    //void creditTiedot(QNetworkReply *reply); // Jos tarvitset, poista kommentti
    void btnDEBITClicked();
    //void ChooseCardSlot();
    void btnCREDITClicked();
    void handleDebit();
    void handleCredit();
    void handleNetworkError(QNetworkReply::NetworkError error);

private:
    Ui::ChooseCard *ui;
    QNetworkAccessManager *manager;
    QByteArray token;
    QNetworkReply *reply;
    QByteArray chooseCard;
    void openAccount(const QJsonObject &obj);
    QString username;
};

#endif // CHOOSECARD_H

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
    //void cardSelected(QString type); //Välittää tiedon onko credit vai debit


private slots:
    //void on_btn_Credit_clicked();

    //void on_btn_Debit_clicked();
    //void debitTiedot(QNetworkReply *reply);
    //void creditTiedot(QNetworkReply *reply);
    void btnDEBITClicked();
    void ChooseCardSlot();
    void btnCREDITClicked();
    void handleDebit();
    void handleCredit();





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

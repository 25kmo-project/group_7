#ifndef TRANSFER_H
#define TRANSFER_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class transfer;
}

class transfer : public QDialog
{
    Q_OBJECT

public:
    explicit transfer(QWidget *parent = nullptr);
    ~transfer();

    void setAccountId(int id);
    void setToken(const QString &t);
    QString balance;
    int accountID = -1;
    QString token;

private:
    Ui::transfer *ui;
    QNetworkAccessManager *manager;

    // QString balance;

    void onTransferReply(QNetworkReply *reply);

private slots:
    void btnBackClicked();
    void btnTransferClicked();


signals:
    void transferSuccesful();

protected:
    void showEvent(QShowEvent *event) override;



};

#endif // TRANSFER_H

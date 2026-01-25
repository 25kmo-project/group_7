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

    QString balance;

private:
    Ui::transfer *ui;

private slots:



};

#endif // TRANSFER_H

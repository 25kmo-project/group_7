#include "transactions.h"
#include "ui_transactions.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsonarray.h>
#include <QStandardItemModel>

#include "apiclient.h"

Transactions::Transactions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Transactions)
{
    ui->setupUi(this);

    connect(ui->btnClose, &QPushButton::clicked, this, &Transactions::btnCloseClicked);
}

Transactions::~Transactions()
{
    delete ui;
}

void Transactions::setToken(const QString &t)
{
    token = t;
}

void Transactions::setAccountId(int id)
{
    accountId = id;
}

void Transactions::btnPreviousClicked()
{
    if(currentOffset >= pageSize) {
        currentOffset -= pageSize;
        updateLogs();
    }
}

void Transactions::btnNextClicked()
{
    currentOffset += pageSize;
    updateLogs();
}

void Transactions::getLogsSlot()
{

}

void Transactions::updateLogs()
{
    QString endpoint = "bank_log/paginated/" + QString::number(accountId) + "?limit=" + QString::number(pageSize) + "&offset=" + QString::number(currentOffset);

    auto reply = ApiClient::instance().get(endpoint);

    connect(reply, &QNetworkReply::finished, this, [this, reply]()  {
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray response_data = reply->readAll();
            QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
            QJsonArray json_array = json_doc.array();

            QStandardItemModel *model = new QStandardItemModel(0, 3, this);
            model->setHeaderData(0, Qt::Horizontal, "Päivämäärä");
            model->setHeaderData(1, Qt::Horizontal, "Tapahtuma");
            model->setHeaderData(2, Qt::Horizontal, "Summa");

            for (const QJsonValue &value : json_array) {
                QJsonObject obj = value.toObject();

                QString rawDate = obj["event_time"].toString();
                QString formattedDate = QDateTime::fromString(rawDate, Qt::ISODate).toString("dd.MM.yy hh:mm");

                QList<QStandardItem*> row;
                row << new QStandardItem(formattedDate);
                row << new QStandardItem(obj["actions"].toString());
                row << new QStandardItem(obj["amount"].toString() + " €");
                model->appendRow(row);
            }

            ui->tableTransactions->setModel(model);
            ui->tableTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            ui->btnNext->setEnabled(json_array.size() == pageSize);
            ui->btnPrevious->setEnabled(currentOffset > 0);
        }else{
            qDebug() << "API virhe:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void Transactions::btnCloseClicked()
{
    close();
}

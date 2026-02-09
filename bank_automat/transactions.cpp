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
    connect(ui->btnOlder, &QPushButton::clicked, this, &Transactions::btnOlderClicked);
    connect(ui->btnNewer, &QPushButton::clicked, this, &Transactions::btnNewerClicked);
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

void Transactions::btnNewerClicked()
{
    if(currentOffset >= pageSize) {
        currentOffset -= pageSize;
        updateLogs();
    }
}

void Transactions::btnOlderClicked()
{
    currentOffset += pageSize;
    updateLogs();
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

                QString action = obj["actions"].toString();
                QString translatedAction = action;
                QColor actionColor;

                if(action == "deposit"){
                    translatedAction = "Talletus";
                    actionColor = QColor(0, 255, 0);
                }else if(action == "withdrawal"){
                    translatedAction = "Nosto";
                    actionColor = QColor(255, 49, 49);
                }else if(action == "transfer"){
                    translatedAction = "Tilisiirto";
                    actionColor = QColor(0, 255, 255);
                }

                QStandardItem *actionItem = new QStandardItem(translatedAction);
                actionItem->setForeground(QBrush(actionColor));

                QString rawDate = obj["event_time"].toString();
                QDateTime dateTime = QDateTime::fromString(rawDate, Qt::ISODate);
                dateTime = dateTime.toLocalTime();
                QString formattedDate = dateTime.toString("dd.MM.yy hh:mm");

                QList<QStandardItem*> row;
                row << new QStandardItem(formattedDate);
                row << actionItem;
                row << new QStandardItem(obj["amount"].toString() + " €");
                model->appendRow(row);
            }

            ui->tableTransactions->setModel(model);
            ui->tableTransactions->verticalHeader()->hide();
            ui->tableTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            ui->btnOlder->setEnabled(json_array.size() == pageSize);
            ui->btnNewer->setEnabled(currentOffset > 0);
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

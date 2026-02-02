#include "transactions.h"
#include "ui_transactions.h"

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

}

void Transactions::btnNextClicked()
{

}

void Transactions::btnCloseClicked()
{
    close();
}

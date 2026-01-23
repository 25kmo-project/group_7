#include "deposit.h"
#include "ui_deposit.h"

Deposit::Deposit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Deposit)
{
    ui->setupUi(this);
    manager=new QNetworkAccessManager (this);
    connect(ui->btnDepositBack, &QPushButton::clicked, this, &Deposit::btnDepositBackClicked);
    connect(ui->btnAddFive, &QPushButton::clicked, this, &Deposit::btnAddFiveClicked);
    connect(ui->btnAddTen, &QPushButton::clicked, this, &Deposit::btnAddTenClicked);
    connect(ui->btnAddTwenty, &QPushButton::clicked, this, &Deposit::btnAddTwentyClicked);
    connect(ui->btnAddFifty, &QPushButton::clicked, this, &Deposit::btnAddFiftyClicked);
    connect(ui->btnAddHundred, &QPushButton::clicked, this, &Deposit::btnAddHundredClicked);
    connect(ui->btnAddTwoHundred, &QPushButton::clicked, this, &Deposit::btnAddTwoHundredClicked);
    connect(ui->btnAddFiveHundred, &QPushButton::clicked, this, &Deposit::btnAddFiveHundredClicked);
    connect(ui->btnReset, &QPushButton::clicked, this, &Deposit::btnResetClicked);
}

Deposit::~Deposit()
{
    delete ui;
}

void Deposit::btnDepositBackClicked()
{
    this->close();
}

void Deposit::btnAddFiveClicked()
{
    currentAmount += 5;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddTenClicked()
{
    currentAmount += 10;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddTwentyClicked()
{
    currentAmount += 20;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddFiftyClicked()
{
    currentAmount +=50;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddHundredClicked()
{
    currentAmount +=100;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddTwoHundredClicked()
{
    currentAmount +=200;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnAddFiveHundredClicked()
{
    currentAmount +=500;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

void Deposit::btnNewDepositClicked()
{

}

void Deposit::btnResetClicked()
{
    currentAmount = 0;
    ui->labelAddedAmount->setText(QString::number(currentAmount) + " €");
}

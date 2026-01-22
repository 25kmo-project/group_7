#include "deposit.h"
#include "ui_deposit.h"

Deposit::Deposit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Deposit)
{
    ui->setupUi(this);
    connect(ui->btnDepositBack, &QPushButton::clicked, this, &Deposit::btnDepositBackClicked);
}

Deposit::~Deposit()
{
    delete ui;
}

void Deposit::btnDepositBackClicked()
{
    this->close();
}

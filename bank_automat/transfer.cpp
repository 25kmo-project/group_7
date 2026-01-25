#include "transfer.h"
#include "ui_transfer.h"

transfer::transfer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::transfer)
{
    ui->setupUi(this);
}

transfer::~transfer()
{
    delete ui;
}

#include "choosecard.h"
#include "ui_choosecard.h"

ChooseCard::ChooseCard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChooseCard)
{
    ui->setupUi(this);
}

ChooseCard::~ChooseCard()
{
    delete ui;
}

void ChooseCard::on_btn_Credit_clicked()
{
    //cardSelected("credit");
    qDebug() << "Credit painettu";

}


void ChooseCard::on_btn_Debit_clicked()
{
    //cardSelected("Debit");
    qDebug() << "Debit painettu";

}


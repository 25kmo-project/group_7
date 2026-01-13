#include "pin.h"
#include "ui_pin.h"

Pin::Pin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Pin)
{
    ui->setupUi(this);
}

Pin::~Pin()
{
    delete ui;
}

void Pin::on_btn_ok_clicked()
{

}


void Pin::on_btn_cancel_clicked()
{

}


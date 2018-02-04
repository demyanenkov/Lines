#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_checkBox_clicked(bool checked)
{
    if(checked)
        Dialog::showFullScreen();
    else
        Dialog::showNormal();
}

void Dialog::on_pushButton_2_clicked()
{
    exit(0);
}

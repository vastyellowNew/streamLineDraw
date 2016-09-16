#include "choosemap.h"
#include "ui_choosemap.h"

chooseMap::chooseMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseMap)
{
    ui->setupUi(this);
    this->ui->radioButton->setChecked(true);
}

chooseMap::~chooseMap()
{
    delete ui;
}

void chooseMap::on_buttonBox_accepted()
{
    emit mapNo(mChoice);
}

void chooseMap::on_radioButton_clicked(bool checked)
{
    if (checked) mChoice = 0;
}

void chooseMap::on_radioButton_3_clicked(bool checked)
{
    if (checked) mChoice = 2;
}

void chooseMap::on_radioButton_4_clicked(bool checked)
{
    if (checked) mChoice = 3;
}

void chooseMap::on_radioButton_5_clicked(bool checked)
{
    if (checked) mChoice = 4;
}

void chooseMap::on_radioButton_6_clicked(bool checked)
{
    if (checked) mChoice = 5;
}

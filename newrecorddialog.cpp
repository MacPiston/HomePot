#include "newrecorddialog.h"
#include "ui_newrecorddialog.h"

newRecordDialog::newRecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newRecordDialog)
{
    ui->setupUi(this);
}

newRecordDialog::~newRecordDialog()
{
    delete ui;
}

void newRecordDialog::on_cancelPushButton_clicked()
{
    this->hide();
}

#include "userchangedialog.h"
#include "ui_userchangedialog.h"

UserChangeDialog::UserChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserChangeDialog)
{
    ui->setupUi(this);
}

UserChangeDialog::~UserChangeDialog()
{
    delete ui;
}

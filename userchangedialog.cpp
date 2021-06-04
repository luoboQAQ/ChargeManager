#include "userchangedialog.h"
#include "ui_userchangedialog.h"

UserChangeDialog::UserChangeDialog(QString aid, QWidget *parent) : QDialog(parent),
                                                                   ui(new Ui::UserChangeDialog)
{
    ui->setupUi(this);
}

UserChangeDialog::~UserChangeDialog()
{
    delete ui;
}

//设置文本框可行性
void UserChangeDialog::SetAble(int arg)
{
    bool enable = true;
    if (arg == 0)
        enable = false;
    else if (arg == 1)
        enable = true;
    ui->m_NameEdit->setEnabled(enable);
    ui->m_PasswdEdit->setEnabled(enable);
    ui->m_CardidEdit->setEnabled(enable);
    ui->m_SdcEdit->setEnabled(enable);
    ui->m_SBox->setEnabled(enable);
    ui->m_CBox->setEnabled(enable);
}

//点击删除选项
void UserChangeDialog::on_m_delRBtn_clicked()
{
    if (ui->m_delRBtn->isChecked())
    {
        choose = 2;
        SetAble(0);
    }
}

//点击修改选项
void UserChangeDialog::on_m_changeRBtn_clicked()
{
    if (ui->m_changeRBtn->isChecked())
    {
        choose = 1;
        SetAble(1);
    }
}

//点击增加选项
void UserChangeDialog::on_m_addRBtn_clicked()
{
    if (ui->m_addRBtn->isChecked())
    {
        choose = 0;
        SetAble(1);
    }
}

void UserChangeDialog::on_m_yesBtn_clicked()
{
}

void UserChangeDialog::on_m_autoBtn_clicked()
{
}

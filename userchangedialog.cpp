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
    ui->m_ClassEdit->setEnabled(enable);
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

//添加学生信息
void UserChangeDialog::AddStu()
{
}

//修改学生信息
void UserChangeDialog::ChangeStu()
{
}

//删除学生信息
void UserChangeDialog::DelStu()
{
}

//点击确认按钮
void UserChangeDialog::on_m_yesBtn_clicked()
{
    switch (choose)
    {
    case 0:
        AddStu();
        break;
    case 1:
        ChangeStu();
        break;
    case 2:
        DelStu();
        break;
    default:
        break;
    }
}

//点击自动补全按钮
void UserChangeDialog::on_m_autoBtn_clicked()
{
    if (choose != 1)
        return;
    QString sno = ui->m_SnoEdit->text();
    if (sno.isEmpty())
        return;
    QSqlQuery query;
    QString str = QString("SELECT sname,sdc,sclass,sage,ssex "
                          "FROM student "
                          "WHERE sno='%1'")
                      .arg(sno);
    if (!GetQuery(str, query))
        return;
    ui->m_NameEdit->setText(query.value(0).toString());
    ui->m_SdcEdit->setText(query.value(1).toString());
    ui->m_ClassEdit->setText(query.value(2).toString());
    ui->m_SBox->setValue(query.value(3).toInt());
    ui->m_CBox->setCurrentText(query.value(4).toString());
    str = QString("SELECT cardid FROM card WHERE sno='%1'").arg(sno);
    if (!GetQuery(str, query))
        return;
    ui->m_CardidEdit->setText(query.value(0).toString());
}

//执行SQL语句
bool UserChangeDialog::GetQuery(QString &str, QSqlQuery &query)
{
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }
    return true;
}
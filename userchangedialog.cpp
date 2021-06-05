#include "userchangedialog.h"
#include "ui_userchangedialog.h"

UserChangeDialog::UserChangeDialog(QString aid, QWidget *parent) : QDialog(parent),
                                                                   ui(new Ui::UserChangeDialog)
{
    ui->setupUi(this);
    admin_id=aid;
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
{ QString sno = ui->m_SnoEdit->text();
    QSqlQuery query;
    QString str = QString("SELECT * FROM student WHERE sno=%1").arg(sno);
      if (!query.exec(str))
    {
        qDebug("查询学生信息失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return ;
    }
    if (!query.next())
    {
        QMessageBox::information(this, "提示", "没有该学号！");
        return ;
    }


    str = QString("SELECT is_using FROM record,card WHERE sno=%1 AND is_using=1").arg(sno);
    if (!query.exec(str))
    {
        qDebug("查询上机状态失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return ;
    }
    if (query.next())
    {
        QMessageBox::information(this, "提示", "上机状态下不能删除！");
        return ;
    }
    if (QMessageBox::question(this, "删除", "你是否确定要删除？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return ;

    str=QString("DELETE FROM user_record "
                  "WHERE sno='%1'").arg(sno);
        if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "删除失败！");
        qDebug() << query.lastError();
        return ;
    }
    QMessageBox::information(this, "提示", "删除成功！");
    return ;
    QDateTime current_time = QDateTime::currentDateTime();
    QString stime = current_time.toString("yyyy-MM-ddThh:mm:ss");
    str = QString("INSERT INTO  user_record "
                  "VALUES('%1','%2','0','%3')")
              .arg(stime)
              .arg(admin_id)
              .arg(sno);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "更新失败！");
        qDebug() << query.lastError();
        return ;
    }
    QMessageBox::information(this, "提示", "更新成功！");
    return ;
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
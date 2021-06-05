#include "userchangedialog.h"
#include "ui_userchangedialog.h"

UserChangeDialog::UserChangeDialog(QString aid, QWidget *parent) : QDialog(parent),
                                                                   ui(new Ui::UserChangeDialog)
{
    ui->setupUi(this);
    admin_id = aid;
    choose = 0;
}

UserChangeDialog::~UserChangeDialog()
{
    delete ui;
}

//关闭窗体事件
void UserChangeDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
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
    QSqlQuery query;
    QString str;
    QString sno = ui->m_SnoEdit->text();
    if (sno.isEmpty())
    {
        QMessageBox::warning(this, "错误", "学号不能为空！");
        return;
    }
    QString sname = ui->m_NameEdit->text();
    if (sname.isEmpty())
    {
        QMessageBox::warning(this, "错误", "姓名不能为空！");
        return;
    }
    QString spasswd = ui->m_PasswdEdit->text();
    if (spasswd.isEmpty())
    {
        QMessageBox::warning(this, "错误", "密码不能为空！");
        return;
    }
    QString cardid = ui->m_CardidEdit->text();
    if (cardid.isEmpty())
    {
        QMessageBox::warning(this, "错误", "卡号不能为空！");
        return;
    }
    QString sdc = ui->m_SdcEdit->text();
    if (sdc.isEmpty())
    {
        QMessageBox::warning(this, "错误", "专业不能为空！");
        return;
    }
    QString sclass = ui->m_ClassEdit->text();
    if (sclass.isEmpty())
    {
        QMessageBox::warning(this, "错误", "班级不能为空！");
        return;
    }
    int sage = ui->m_SBox->value();
    QString ssex = ui->m_CBox->currentText();

    QSqlQuery query_insert_s;
    QSqlQuery query_insert_c;
    QSqlQuery query_insert_l;
    QSqlQuery query_do;
    bool ok1 = true;
    bool ok2 = true;
    bool ok3 = true;
    query.exec("SET AUTOCOMMIT=0");  //设置为不自动提交，因为MYSQL默认立即执行
    query.exec("START TRANSACTION"); //开始事务
    str = QString("INSERT INTO student(sno,sname,spasswd,sdc,sclass,sage,ssex) "
                  "VALUES('%1','%2','%3','%4','%5','%6','%7')")
              .arg(sno)
              .arg(sname)
              .arg(spasswd)
              .arg(sdc)
              .arg(sclass)
              .arg(sage)
              .arg(ssex);
    if (!query_insert_s.exec(str))
    {
        QString err = query_insert_s.lastError().databaseText();
        if (err.indexOf("student.PRIMARY") >= 0)
            QMessageBox::warning(this, "错误", "学号重复！");
        else
            QMessageBox::warning(this, "错误", err);
        ok1 = false;
    }

    QDateTime current_time = QDateTime::currentDateTime();
    QString createday = current_time.toString("yyyy-MM-ddThh:mm:ss");
    str = QString("INSERT INTO card(cardid,createday,sno,banlance,state) "
                  "VALUES('%1','%2','%3',0,1)")
              .arg(cardid)
              .arg(createday)
              .arg(sno);
    if (!query_insert_c.exec(str))
    {
        QString err = query_insert_c.lastError().databaseText();
        if (err.indexOf("card.PRIMARY") >= 0)
            QMessageBox::warning(this, "错误", "卡号重复！");
        else
            QMessageBox::warning(this, "错误", err);
        ok2 = false;
    }

    str = QString("INSERT INTO user_record(stime,aid,change_way,sno) "
                  "VALUES('%1','%2','0','%3')")
              .arg(createday)
              .arg(admin_id)
              .arg(sno);
    ok3 = query_insert_l.exec(str);
    if (ok1 && ok2 && ok3)
    {
        query_do.exec("COMMIT"); //提交
        QMessageBox::information(this, "提示", "插入成功！");
    }
    else
    {
        query_do.exec("ROLLBACK"); //回滚
        QMessageBox::warning(this, "错误", "插入失败！");
    }
}

//修改学生信息
void UserChangeDialog::ChangeStu()
{
    QString sno = ui->m_SnoEdit->text();
    QSqlQuery query;
    QString str = QString("SELECT cardid FROM card WHERE sno=%1").arg(sno);
    if (!query.exec(str))
    {
        qDebug("查询学生信息失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    if (!query.next())
    {
        QMessageBox::information(this, "提示", "没有该学号！");
        return;
    }
    QString cardid = query.value(0).toString();

    str = QString("SELECT is_using FROM record WHERE cardid=%1 AND is_using=1").arg(cardid);
    if (!query.exec(str))
    {
        qDebug("查询上机状态失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    if (query.next())
    {
        QMessageBox::information(this, "提示", "上机状态下不能修改！");
        return;
    }
    if (QMessageBox::question(this, "修改", "你是否确定要修改？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    QString sname = ui->m_NameEdit->text();
    QString spasswd = ui->m_PasswdEdit->text();
    QString scardid = ui->m_CardidEdit->text();
    str = QString("SELEXT * FROM card WHERE cardid='%1'").arg(scardid);
    if (query.exec(str))
    {
        QMessageBox::warning(this, "错误", "卡号有重复！");
        return;
    }
    if (scardid.isEmpty())
        scardid = cardid;
    QString sdc = ui->m_SdcEdit->text();
    QString scl = ui->m_ClassEdit->text();
    int sage = ui->m_SBox->value();
    QString ssex = ui->m_CBox->currentText();

    str = QString("SELECT sname,sdc,sclass,sage,ssex,spasswd "
                  "FROM student "
                  "WHERE sno='%1'")
              .arg(sno);
    if (!GetQuery(str, query))
        return;
    if (sname.isEmpty())
        sname = query.value(0).toString();
    if (sdc.isEmpty())
        sdc = query.value(1).toString();
    if (scl.isEmpty())
        scl = query.value(2).toString();
    if (sage == 0)
        sage = query.value(3).toInt();
    if (spasswd.isEmpty())
        spasswd = query.value(5).toString();
    str = QString("UPDATE student SET sname='%1' "
                  "WHERE sno='%2';"
                  "UPDATE student SET spasswd='%3' "
                  "WHERE sno='%2';"
                  "UPDATE student SET sdc='%4' "
                  "WHERE sno='%2';"
                  "UPDATE student SET sclass='%5' "
                  "WHERE sno='%2';"
                  "UPDATE student SET sage='%6' "
                  "WHERE sno='%2';"
                  "UPDATE student SET ssex='%7' "
                  "WHERE sno='%2';"
                  "UPDATE card SET cardid='%8' "
                  "WHERE sno='%2';")
              .arg(sname)
              .arg(sno)
              .arg(spasswd)
              .arg(sdc)
              .arg(scl)
              .arg(sage)
              .arg(ssex)
              .arg(scardid);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "修改失败！");
        qDebug() << query.lastError();
        return;
    }
    QDateTime current_time = QDateTime::currentDateTime();
    QString stime = current_time.toString("yyyy-MM-ddThh:mm:ss");
    str = QString("INSERT INTO  user_record(stime,aid,change_way,sno) "
                  "VALUES('%1','%2','1','%3')")
              .arg(stime)
              .arg(admin_id)
              .arg(sno);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "更新失败！");
        qDebug() << query.lastError();
        return;
    }
    QMessageBox::information(this, "提示", "修改成功！");
}

//删除学生信息
void UserChangeDialog::DelStu()
{
    QString sno = ui->m_SnoEdit->text();
    QSqlQuery query;
    QString str = QString("SELECT cardid FROM card WHERE sno=%1").arg(sno);
    if (!query.exec(str))
    {
        qDebug("查询学生信息失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    if (!query.next())
    {
        QMessageBox::information(this, "提示", "没有该学号！");
        return;
    }
    QString cardid = query.value(0).toString();
    str = QString("SELECT is_using FROM record WHERE cardid=%1 AND is_using=1").arg(cardid);
    if (!query.exec(str))
    {
        qDebug("查询上机状态失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    if (query.next())
    {
        QMessageBox::information(this, "提示", "上机状态下不能删除！");
        return;
    }
    if (QMessageBox::question(this, "删除", "你是否确定要删除？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    str = QString("DELETE FROM student "
                  "WHERE sno='%1'")
              .arg(sno);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "删除失败！");
        qDebug() << query.lastError();
        return;
    }
    QDateTime current_time = QDateTime::currentDateTime();
    QString stime = current_time.toString("yyyy-MM-ddThh:mm:ss");
    str = QString("INSERT INTO  user_record(stime,aid,change_way,sno) "
                  "VALUES('%1','%2','2','%3')")
              .arg(stime)
              .arg(admin_id)
              .arg(sno);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "更新失败！");
        qDebug() << query.lastError();
        return;
    }
    QMessageBox::information(this, "提示", "删除成功！");
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
    QString str = QString("SELECT sname,sdc,sclass,sage,ssex,spasswd "
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
    ui->m_PasswdEdit->setText(query.value(5).toString());
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
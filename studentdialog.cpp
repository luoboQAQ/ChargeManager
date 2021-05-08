#include "studentdialog.h"
#include "ui_studentdialog.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

StudentDialog::StudentDialog(QString user, QWidget *parent) : QDialog(parent),
                                                              ui(new Ui::StudentDialog)
{
    // 设置窗体有最小化按钮
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    ui->setupUi(this);

    //设置个人信息区
    ui->s_NoLabel->setText(user);
    QSqlQuery query;
    QString str = QString("SELECT sname,sdc,sclass FROM student WHERE sno='%1'").arg(user);
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    ui->s_NameLabel->setText(query.value(0).toString());
    ui->s_DnameLabel->setText(query.value(1).toString());
    ui->s_ClassLabel->setText(query.value(2).toString());

    //设置上机卡信息区
    str = QString("SELECT cardid,createday,banlance,state FROM card WHERE sno='%1'").arg(user);
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    ui->c_NoLabel->setText(query.value(0).toString());
    ui->c_DateLabel->setText(query.value(1).toString().split('T').at(0));
    ui->c_BanlaLabel->setText(query.value(2).toString());
    if (query.value(3).toBool())
        ui->c_OkLabel->setText("正常");
    else
        ui->c_OkLabel->setText("挂失");
}

StudentDialog::~StudentDialog()
{
    delete ui;
}

//登录槽函数
void StudentDialog::on_loginBtn_clicked()
{
}

//查询槽函数
void StudentDialog::on_searchBtn_clicked()
{
}

//挂失槽函数
void StudentDialog::on_reportLossBtn_clicked()
{
    if (QMessageBox::question(this, "挂失", "你是否确定要挂失？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;
}

//充值槽函数
void StudentDialog::on_rechargeBtn_clicked()
{
}

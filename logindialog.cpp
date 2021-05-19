#include "logindialog.h"
#include "./ui_logindialog.h"
#include "studentdialog.h"
#include "admindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog)
{
    // 设置窗体有最小化按钮
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtnBox_accepted()
{
    QString in_user = ui->userEdit->text();
    QString in_passwd = ui->passwordEdit->text();
    QSqlQuery query;
    QString str = QString("SELECT passwd,tag FROM login WHERE user='%1'").arg(in_user);
    if (!query.exec(str))
    {
        qDebug("查询密码失败！");
        qDebug() << query.lastError();
    }
    if (query.next())
    {
        QString passwd = query.value(0).toString();
        int tag = query.value(1).toInt();
        if (passwd == in_passwd)
        {
            if (tag == 1)
            {
                StudentDialog *sd = new StudentDialog(in_user);
                sd->show();
                this->close();
            }
            else if (tag == 0)
            {
                AdminDialog *ad = new AdminDialog(in_user);
                ad->show();
                this->close();
            }
        }
        else
            QMessageBox::warning(this, "错误", "密码错误！");
    }
    else
        QMessageBox::warning(this, "错误", "用户不存在！");
}

void LoginDialog::on_loginBtnBox_rejected()
{
    //调试用
    QString in_user = ui->userEdit->text();
    AdminDialog *ad = new AdminDialog(in_user);
    ad->show();
    this->close();
    return;
    //end
    close();
}

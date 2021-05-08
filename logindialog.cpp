#include "logindialog.h"
#include "./ui_logindialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>

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
    QString str = QString("SELECT password FROM sa WHERE user='%1'").arg(in_user);
    if (!query.exec(str))
        qDebug() << query.lastError();
    if (query.next())
    {
        QString passwd = query.value(0).toString();
        if (passwd == in_passwd)
            qDebug() << "登陆成功！";
        else
            qDebug() << "登陆失败！";
    }
}

void LoginDialog::on_loginBtnBox_rejected()
{
    close();
}

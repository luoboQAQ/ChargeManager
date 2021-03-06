#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "admindialog.h"
#include "studentdialog.h"
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LoginDialog;
}
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginBtnBox_accepted();

    void on_loginBtnBox_rejected();

private:
    Ui::LoginDialog *ui;
};
#endif // LOGINDIALOG_H

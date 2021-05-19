#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

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

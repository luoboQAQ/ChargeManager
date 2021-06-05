#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include "logindialog.h"
#include "userchangedialog.h"
#include <QCloseEvent>
#include <QDialog>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardItemModel>

namespace Ui
{
    class AdminDialog;
}

class AdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminDialog(QString user, QWidget *parent = nullptr);
    ~AdminDialog();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_i_Btn_clicked();

    void on_l_Btn_clicked();

    void on_c_Btn_clicked();

    void on_i_checkBox_stateChanged(int arg1);

    void on_m_QueryBtn_clicked();

    void on_m_addBtn_clicked();

    void DialogClosed();

private:
    bool SetupName();
    bool GetComputerNums();
    bool GetQuery(QString &str, QSqlQuery &query);
    bool GetUserName(QString user);
    bool Q_vNum(QString user, QDate date);
    bool Q_atime(QString user, QDate date);
    bool Q_avgtime(QString user, QDate date);
    bool Q_charge(QString user, QDate date);
    bool Q_income(QString user, QDate date);
    bool Q_loss(QString user, QDate date);
    bool SetModel(QSqlQuery &query, QStringList &title);
    int L_loss(QString cardid);
    bool L_reloss(QString cardid);
    bool L_fing(QString user);

private:
    Ui::AdminDialog *ui;
    bool isAllDate;
    QStandardItemModel model;
    QString admin_id;
    UserChangeDialog *userdialog;
};
#endif // ADMINDIALOG_H

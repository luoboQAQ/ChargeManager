#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include "logindialog.h"
#include "squerydialog.h"
#include <QCloseEvent>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime>
#include <QTimer>

namespace Ui
{
    class StudentDialog;
}

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudentDialog(QString user, QWidget *parent = nullptr);
    ~StudentDialog();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_loginBtn_clicked();

    void on_searchBtn_clicked();

    void on_reportLossBtn_clicked();

    void on_rechargeBtn_clicked();

    void onTimeOut(void);

    void DialogClosed();

private:
    Ui::StudentDialog *ui;
    QString computer_id;
    bool isOnline;
    bool isCardOk;
    double banlance;
    double cost;
    QString cardid;
    QString serial_num; //本次上机的流水号
    QString userid;     //sno
    QTimer m_timer;

    int stime; //秒
    int mtime; //分钟
    SQueryDialog *querydialog;
};

#endif // STUDENTDIALOG_H

#ifndef SQUERYDIALOG_H
#define SQUERYDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardItemModel>
namespace Ui
{
    class SQueryDialog; 
}

class SQueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQueryDialog(QString cardid, QWidget *parent = nullptr);
    ~SQueryDialog();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_i_Btn_clicked();
    void on_i_checkBox_stateChanged(int arg1);

signals:
    void windowClosed(); //信号发送函数

private:
    Ui::SQueryDialog *ui;
    bool isAllDate;
    QStandardItemModel model;
    QString admin_id;
    QString cardid;
private:
  
    bool GetUserName(QString user);
    bool Q_vNum( QDate date);
    bool Q_atime(QDate date);
    bool Q_avgtime( QDate date);
    bool Q_charge(QDate date);
    bool Q_income(QDate date);
    bool Q_loss( QDate date);
    bool SetModel(QSqlQuery &query, QStringList &title);
};

#endif // SQUERYDIALOG_H

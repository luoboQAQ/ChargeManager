#ifndef USERCHANGEDIALOG_H
#define USERCHANGEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QCloseEvent>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui
{
    class UserChangeDialog;
}

class UserChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserChangeDialog(QString aid, QWidget *parent = nullptr);
    ~UserChangeDialog();

private slots:
    void on_m_delRBtn_clicked();

    void on_m_changeRBtn_clicked();

    void on_m_addRBtn_clicked();

    void on_m_yesBtn_clicked();

    void on_m_autoBtn_clicked();

private:
    void SetAble(int arg);
    bool GetQuery(QString &str, QSqlQuery &query);

private:
    Ui::UserChangeDialog *ui;
    int choose = -1;
};

#endif // USERCHANGEDIALOG_H

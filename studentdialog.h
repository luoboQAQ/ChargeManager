#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>

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

private slots:
    void on_loginBtn_clicked();

    void on_searchBtn_clicked();

    void on_reportLossBtn_clicked();

    void on_rechargeBtn_clicked();

private:
    Ui::StudentDialog *ui;
};

#endif // STUDENTDIALOG_H

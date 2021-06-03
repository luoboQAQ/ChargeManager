#ifndef USERCHANGEDIALOG_H
#define USERCHANGEDIALOG_H

#include <QDialog>

namespace Ui {
class UserChangeDialog;
}

class UserChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserChangeDialog(QWidget *parent = nullptr);
    ~UserChangeDialog();

private:
    Ui::UserChangeDialog *ui;
};

#endif // USERCHANGEDIALOG_H

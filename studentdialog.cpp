#include "studentdialog.h"
#include "ui_studentdialog.h"

StudentDialog::StudentDialog(QString user, QWidget *parent) : QDialog(parent),
                                                              ui(new Ui::StudentDialog)
{
    // 设置窗体有最小化按钮
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    ui->setupUi(this);
}

StudentDialog::~StudentDialog()
{
    delete ui;
}

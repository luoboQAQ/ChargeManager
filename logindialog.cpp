#include "logindialog.h"
#include "./ui_logindialog.h"

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

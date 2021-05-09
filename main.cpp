#include "logindialog.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>

bool opendb();

int main(int argc, char *argv[])
{
    opendb();
    QApplication a(argc, argv);
    LoginDialog w;
    w.show();
    return a.exec();
}

bool opendb()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("chm"); //数据库名
    db.setUserName("root");
    db.setPassword("root");
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),
                              "无法创建数据库连接！ ", QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug("成功打开数据库！");
        return true;
    }
}

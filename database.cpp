#include "database.h"

Database::Database()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("test1"); //数据库名
    db.setUserName("root");
    db.setPassword("root");
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),
                              "无法创建数据库连接！ ", QMessageBox::Cancel);
    }
    else
        qDebug("成功打开数据库！");
}

Database::~Database()
{
}
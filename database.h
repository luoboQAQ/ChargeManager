#ifndef __DATABASH_H
#define __DATABASH_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

class Database
{
private:
    QSqlDatabase db;      //建立Qt和数据库的链接
    QSqlQueryModel model; //保存结果集
public:
    Database();
    ~Database();
};

#endif
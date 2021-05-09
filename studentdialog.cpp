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

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    //设置个人信息区
    ui->s_NoLabel->setText(user);
    QSqlQuery query;
    QString str = QString("SELECT sname,sdc,sclass FROM student WHERE sno='%1'").arg(user);
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    ui->s_NameLabel->setText(query.value(0).toString());
    ui->s_DnameLabel->setText(query.value(1).toString());
    ui->s_ClassLabel->setText(query.value(2).toString());

    //设置上机卡信息区
    str = QString("SELECT cardid,createday,banlance,state FROM card WHERE sno='%1'").arg(user);
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    cardid = query.value(0).toString();
    ui->c_NoLabel->setText(cardid);
    ui->c_DateLabel->setText(query.value(1).toString().split('T').at(0));
    banlance = query.value(2).toDouble();
    ui->c_BanlaLabel->setText(QString::number(banlance, 'f', 2));
    isCardOk = query.value(3).toBool();
    if (isCardOk)
        ui->c_OkLabel->setText("正常");
    else
        ui->c_OkLabel->setText("挂失");

    //分配电脑
    str = QString("SELECT computer_id FROM computer WHERE computer_state='空闲'");
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return;
    }
    computer_id = query.value(0).toString();
    ui->m_NoLabel->setText(computer_id);

    isOnline = false;
}

StudentDialog::~StudentDialog()
{
    if (isOnline)
        on_loginBtn_clicked();
    delete ui;
}

//上下机槽函数
void StudentDialog::on_loginBtn_clicked()
{
    if (!isOnline)
    {
        //判断上机卡状态和余额
        if (!isCardOk)
        {
            QMessageBox::warning(this, "错误", "上机卡已被挂失！");
            return;
        }
        if (banlance < 3)
        {
            QMessageBox::warning(this, "错误", "余额不足3元，请充值！");
            return;
        }

        //生成流水号
        QDateTime current_time = QDateTime::currentDateTime();
        serial_num = current_time.toString("ddhhmmss");
        QString start_time = current_time.toString("yyyy-MM-ddThh:mm:ss");

        //生成上机记录
        QString str = QString("INSERT INTO record"
                              " VALUES('%1','%2','%3','0','1')")
                          .arg(serial_num)
                          .arg(cardid)
                          .arg(start_time);
        QSqlQuery query;
        if (!query.exec(str))
        {
            qDebug("插入数据失败！");
            qDebug() << query.lastError();
            return;
        }

        //设定计时器
        m_timer.start(1000);
        stime = 0;
        mtime = 0;
        cost = 0;
        isOnline = true;

        //更新界面
        ui->loginBtn->setText("下机");
        ui->m_OkLabel->setText("已上线");
        ui->m_TimeLabel->setText(current_time.toString("hh:mm:ss"));
    }
    else
    {
        isOnline = false;
        m_timer.stop();
        //更新上机记录
        QString str = QString("UPDATE record "
                              "SET is_using=0,"
                              "ctime=ctime+'%1' "
                              "WHERE serial_num='%2'")
                          .arg(QString::number(stime))
                          .arg(serial_num);
        QSqlQuery query;
        if (!query.exec(str))
        {
            qDebug("更新数据失败！");
            qDebug() << query.lastError();
            return;
        }
        //更新界面
        ui->loginBtn->setText("上机");
        ui->m_OkLabel->setText("已下线");
    }
}

//每当定时器到时以后执行的函数
void StudentDialog::onTimeOut(void)
{
    stime++;
    if (stime % 60 == 0)
    {
        stime = 0;
        mtime++;
        //更新上机记录表
        QString str = QString("UPDATE record "
                              "SET ctime=ctime+'100' "
                              "WHERE serial_num='%1'")
                          .arg(serial_num);
        QSqlQuery query;
        if (!query.exec(str))
        {
            qDebug("更新在线时间失败！");
            qDebug() << query.lastError();
            return;
        }
        //更新账户余额
        banlance -= 0.1;
        cost += 0.1;
        str = QString("UPDATE card "
                      "SET banlance=banlance-0.1 "
                      "WHERE cardid='%1'")
                  .arg(cardid);
        if (!query.exec(str))
        {
            qDebug("更新余额失败！");
            qDebug() << query.lastError();
            return;
        }
        ui->c_BanlaLabel->setText(QString::number(banlance, 'f', 2));
    }
    //更新界面上的信息
    QString strStime = QString::number(stime);
    QString strMtime = QString::number(mtime);
    if (stime < 10)
        strStime = '0' + strStime;
    if (mtime < 10)
        strMtime = '0' + strMtime;
    ui->m_CostTimeLabel->setText(strMtime + ':' + strStime);
    ui->m_CostLabel->setText(QString::number(cost, 'f', 1) + "元");
    //剩余余额为0时强制下机
    if (banlance <= 0)
        on_loginBtn_clicked();
}

//查询槽函数
void StudentDialog::on_searchBtn_clicked()
{
}

//挂失槽函数
void StudentDialog::on_reportLossBtn_clicked()
{
    if (QMessageBox::question(this, "挂失", "你是否确定要挂失？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;
}

//充值槽函数
void StudentDialog::on_rechargeBtn_clicked()
{
}

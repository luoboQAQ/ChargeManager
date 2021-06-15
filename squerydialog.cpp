#include "squerydialog.h"
#include "ui_squerydialog.h"

SQueryDialog::SQueryDialog(QString cardid, QWidget *parent) : QDialog(parent),
                                                              ui(new Ui::SQueryDialog)
{
    ui->setupUi(this);
    this->cardid = cardid;
    isAllDate = false;
}

SQueryDialog::~SQueryDialog()
{
    delete ui;
}

//关闭窗体事件
void SQueryDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
}

//点击查询按钮
void SQueryDialog::on_i_Btn_clicked()
{
    //遍历6个单选框查看选中
    QRadioButton *qbtn[6] = {ui->i_vNumRBtn, ui->i_atimeRBtn, ui->i_avtimeRBtn,
                             ui->i_incomeRBtn, ui->i_lossRBtn, ui->i_chargeRBtn};
    int index = -1;
    for (int i = 0; i < 6; i++)
    {
        if (qbtn[i]->isChecked())
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        QMessageBox::warning(this, "错误", "未选中某一条查询指令，请选择！");
        return;
    }
    //获取查询日期
    QDate date;
    if (!ui->i_checkBox->isChecked())
        date = ui->i_dateEdit->date();

    switch (index)
    {
    case 0:
        Q_vNum(date);
        break;
    case 1:
        Q_atime(date);
        break;
    case 2:
        Q_avgtime(date);
        break;
    case 3:
        Q_income(date);
        break;
    case 4:
        Q_loss(date);
        break;
    case 5:
        Q_charge(date);
        break;
    default:
        break;
    }
}
//查询上机时长
bool SQueryDialog::Q_atime(QDate date)
{
    QSqlQuery query;
    QString str = "SELECT SEC_TO_TIME(SUM(TIME_TO_SEC(sumtime))) FROM sumtime WHERE 1=1";

    str += QString(" AND cardid='%1'").arg(cardid);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        str += QString(" AND day_time='%1' ").arg(startdate);
    }
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询上机时长失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }

    QStringList title = {"上机总时长"};
    SetModel(query, title);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    return true;
}

//查询平均上机时间
bool SQueryDialog::Q_avgtime(QDate date)
{
    QSqlQuery query;
    QString str = "SELECT SEC_TO_TIME(AVG(TIME_TO_SEC(avgtime))) FROM avgtime WHERE 1=1";

    str += QString(" AND cardid='%1'").arg(cardid);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        str += QString(" AND day_time='%1'").arg(startdate);
    }
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询平均上机时长失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }
    QStringList title = {"上机平均时长"};
    SetModel(query, title);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    return true;
}

//查询充值记录
bool SQueryDialog::Q_charge(QDate date)
{
    QSqlQuery query;
    QString str = "SELECT ctime,cardid,money,opeartor_id FROM charge_record WHERE state=1";

    str += QString(" AND cardid='%1'").arg(cardid);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        QString enddate = date.addDays(1).toString("yyyy-MM-dd");
        str += QString(" AND ctime>='%1' AND ctime<='%2'").arg(startdate).arg(enddate);
    }
    if (!query.exec(str))
    {
        qDebug("查询充值记录失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }
    QStringList title = {"充值时间", "卡号", "充值金额", "操作者ID"};
    SetModel(query, title);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    return true;
}

//查询挂失记录
bool SQueryDialog::Q_loss(QDate date)
{
    QSqlQuery query;
    QString str = QString("SELECT cardid,ltime,opeartor_id,state "
                          "FROM loss_record "
                          "WHERE 1=1");

    str += QString(" AND cardid='%1'").arg(cardid);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        QString enddate = date.addDays(1).toString("yyyy-MM-dd");
        str += QString(" AND ltime>='%1' AND ltime<='%2'").arg(startdate).arg(enddate);
    }
    if (!query.exec(str))
    {
        qDebug("查询挂失记录失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }
    QStringList title = {"卡号", "流水号", "操作者ID", "操作码"};
    SetModel(query, title);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    return true;
}

//查询上机花费
bool SQueryDialog::Q_income(QDate date)
{
    QSqlQuery query;
    QString str = "SELECT SUM(sumcost) FROM sumcost WHERE 1=1";
    str += QString(" AND cardid='%1'").arg(cardid);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        str += QString(" AND day_time='%1' ").arg(startdate);
    }

    if (!query.exec(str) || !query.next())
    {
        qDebug("查询上机花费失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }
    QStringList title = {"总花费"};
    SetModel(query, title);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    return true;
}

//查询上机次数
bool SQueryDialog::Q_vNum(QDate date)
{
    QSqlQuery query;
    QString str = "SELECT SUM(times) FROM times WHERE 1=1";

    str += QString(" AND cardid='%1'").arg(cardid);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        str += QString(" AND day_time='%1'").arg(startdate);
    }
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询上机次数失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }
    QStringList title = {"上机次数"};
    SetModel(query, title);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    return true;
}

//将查询结果转换为能显示的表
bool SQueryDialog::SetModel(QSqlQuery &query, QStringList &title)
{
    model.clear();
    //设置表头
    int maxColumn = title.size();
    model.setColumnCount(maxColumn);
    for (int i = 0; i < maxColumn; i++)
        model.setHeaderData(i, Qt::Horizontal, title.at(i));
    if (!query.first())
    {
        qDebug("此查询为空表");
        return true;
    }
    int row = 0; //行
    do
    {
        for (int i = 0; i < maxColumn; i++)
        {
            QString value = query.value(i).toString();
            if (value.length() == 0)
                value = '0';
            model.setItem(row, i, new QStandardItem(value));
        }
        row++;
    } while (query.next());
    return true;
}
//全部日期复选框状态改变
void SQueryDialog::on_i_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        ui->i_dateEdit->setEnabled(false);
        isAllDate = false;
    }
    else if (arg1 == Qt::Unchecked)
    {
        ui->i_dateEdit->setEnabled(true);
        isAllDate = true;
    }
}
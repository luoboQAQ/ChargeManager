#include "admindialog.h"
#include "ui_admindialog.h"

AdminDialog::AdminDialog(QString user, QWidget *parent) : QDialog(parent),
                                                          ui(new Ui::AdminDialog)
{
    // 设置窗体有最小化按钮
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    ui->setupUi(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表为不可编辑
    admin_id = user;
    userdialog = nullptr;
    SetupName();
    GetComputerNums();
    GetUserName(user);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    m_timer.start(10000);
}

AdminDialog::~AdminDialog()
{
    delete ui;
}

void AdminDialog::onTimeOut()
{
    GetComputerNums();
}

//窗口关闭事件
void AdminDialog::closeEvent(QCloseEvent *event)
{
    LoginDialog *login = new LoginDialog;
    login->show();
}

//获取管理员姓名
bool AdminDialog::GetUserName(QString user)
{
    QSqlQuery query;
    QString name;
    QString str = QString("SELECT aname FROM admin WHERE aid='%1'").arg(user);
    GetQuery(str, query);
    name = query.value(0).toString();
    ui->welcomeLabel->setText(QString("欢迎您，管理员%1！").arg(name));
    return true;
}

//获取当前电脑使用/空闲台数
bool AdminDialog::GetComputerNums()
{
    QSqlQuery query;
    QString str = QString("SELECT COUNT(*) FROM computer WHERE computer_state = '空闲'");
    GetQuery(str, query);
    ui->freeNumLabel->setText(query.value(0).toString());
    str = QString("SELECT COUNT(*) FROM computer WHERE computer_state = '忙碌'");
    GetQuery(str, query);
    ui->busyNumLabel->setText(query.value(0).toString());
    return true;
}

//设置下拉框里的用户名
bool AdminDialog::SetupName()
{
    QSqlQuery query;
    QString str = QString("SELECT sno,sname FROM student LIMIT 0,100");
    GetQuery(str, query);
    QString arg1, arg2;
    QStringList list = {"所有人"};
    do
    {
        arg1 = query.value(0).toString();
        arg2 = query.value(1).toString();
        list += (arg1 + '(' + arg2 + ')');
    } while (query.next());
    ui->i_userCBox->clear();
    ui->l_userCBox->clear();
    ui->c_userCBox->clear();
    ui->i_userCBox->addItems(list);
    list.removeFirst();
    ui->l_userCBox->addItems(list);
    ui->c_userCBox->addItems(list);
    return true;
}

//点击查询按钮
void AdminDialog::on_i_Btn_clicked()
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
    //获取操作对象
    QString user = ui->i_userCBox->currentText();
    if (user != "所有人")
    {
        user = user.split('(').at(0);
        QString str = QString("SELECT cardid FROM card WHERE sno='%1'").arg(user);
        QSqlQuery query;
        GetQuery(str, query);
        user = query.value(0).toString();
    }
    else
        user.clear();
    switch (index)
    {
    case 0:
        Q_vNum(user, date);
        break;
    case 1:
        Q_atime(user, date);
        break;
    case 2:
        Q_avgtime(user, date);
        break;
    case 3:
        Q_income(user, date);
        break;
    case 4:
        Q_loss(user, date);
        break;
    case 5:
        Q_charge(user, date);
        break;
    default:
        break;
    }
}

//查询上机总时长
bool AdminDialog::Q_atime(QString user, QDate date)
{
    QSqlQuery query;
    QString str = "SELECT SEC_TO_TIME(SUM(TIME_TO_SEC(ctime))) FROM record WHERE 1=1";
    if (!user.isNull())
        str += QString(" AND cardid='%1'").arg(user);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        QString enddate = date.addDays(1).toString("yyyy-MM-dd");
        str += QString(" AND stime>='%1' AND stime<='%2'").arg(startdate).arg(enddate);
    }
    GetQuery(str, query);

    QStringList title = {"上机总时长"};
    SetModel(query, title);
    return true;
}

//查询平均上机时间
bool AdminDialog::Q_avgtime(QString user, QDate date)
{
    QSqlQuery query;
    QString str = "SELECT SEC_TO_TIME(AVG(TIME_TO_SEC(ctime))) FROM record WHERE 1=1";
    if (!user.isNull())
        str += QString(" AND cardid='%1'").arg(user);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        QString enddate = date.addDays(1).toString("yyyy-MM-dd");
        str += QString(" AND stime>='%1' AND stime<='%2'").arg(startdate).arg(enddate);
    }
    GetQuery(str, query);
    QStringList title = {"上机平均时长"};
    SetModel(query, title);
    return true;
}

//查询充值记录
bool AdminDialog::Q_charge(QString user, QDate date)
{
    QSqlQuery query;
    QString str = "SELECT ctime,cardid,money,opeartor_id FROM charge_record WHERE state=1";
    if (!user.isNull())
        str += QString(" AND cardid='%1'").arg(user);
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
    return true;
}

//查询挂失记录
bool AdminDialog::Q_loss(QString user, QDate date)
{
    QSqlQuery query;
    QString str = QString("SELECT cardid,ltime,opeartor_id,state "
                          "FROM loss_record "
                          "WHERE 1=1");
    if (!user.isNull())
        str += QString(" AND cardid='%1'").arg(user);
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
    return true;
}

//查询机房收入
bool AdminDialog::Q_income(QString user, QDate date)
{
    QSqlQuery query;
    QString str = "SELECT SUM(cost) FROM record WHERE is_using=0";
    if (!user.isNull())
        str += QString(" AND cardid='%1'").arg(user);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        QString enddate = date.addDays(1).toString("yyyy-MM-dd");
        str += QString(" AND stime>='%1' AND stime<='%2'").arg(startdate).arg(enddate);
    }
    GetQuery(str, query);
    QStringList title = {"收入"};
    SetModel(query, title);
    return true;
}

//查询上机次数
bool AdminDialog::Q_vNum(QString user, QDate date)
{
    QSqlQuery query;
    QString str = "SELECT COUNT(*) FROM record WHERE is_using=0";
    if (!user.isNull())
        str += QString(" AND cardid='%1'").arg(user);
    if (!date.isNull())
    {
        QString startdate = date.toString("yyyy-MM-dd");
        QString enddate = date.addDays(1).toString("yyyy-MM-dd");
        str += QString(" AND stime>='%1' AND stime<='%2'").arg(startdate).arg(enddate);
    }
    GetQuery(str, query);
    QStringList title = {"上机次数"};
    SetModel(query, title);

    return true;
}

//将查询结果转换为能显示的表
bool AdminDialog::SetModel(QSqlQuery &query, QStringList &title)
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
        ui->tableView->setModel(&model);
        ui->tableView->resizeColumnsToContents();
        return true;
    }
    int row = 0; //行
    do
    {
        for (int i = 0; i < maxColumn; i++)
            model.setItem(row, i, new QStandardItem(query.value(i).toString()));
        row++;
    } while (query.next());
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    return true;
}

//点击挂失按钮
void AdminDialog::on_l_Btn_clicked()
{
    int choose = 0;
    if (ui->l_relossRBtn->isChecked())
        choose = 1;
    else if (ui->l_lossRBtn->isChecked())
        choose = 2;
    else
    {
        QMessageBox::warning(this, "错误", "挂失选项没有选中！");
        return;
    }

    //获取用户名
    QString user = ui->l_userCBox->currentText();
    user = user.split('(').at(0);
    //获取卡ID
    QString str = QString("SELECT cardid FROM card WHERE sno='%1'").arg(user);
    QSqlQuery query;
    GetQuery(str, query);
    QString cardid = query.value(0).toString();
    if (choose == 1)
        L_reloss(cardid);
    else if (choose == 2)
        L_loss(cardid);
}

//找回选项
bool AdminDialog::L_reloss(QString cardid)
{
    QSqlQuery query;
    QString str = QString("SELECT state FROM card WHERE cardid=%1").arg(cardid);
    GetQuery(str, query);
    QString state = query.value(0).toString();
    if (state == "1")
    {
        QMessageBox::information(this, "提示", "您并未挂失！");
        return false;
    }
    QDateTime current_time = QDateTime::currentDateTime();
    QString losstime = current_time.toString("yyyy-MM-ddThh:mm:ss");
    str = QString("INSERT INTO loss_record(ltime,cardid,opeartor_id,state) "
                  "VALUES('%1','%2','%3','1')")
              .arg(losstime)
              .arg(cardid)
              .arg(admin_id);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "找回失败！");
        qDebug() << query.lastError();
        return false;
    }
    QMessageBox::information(this, "提示", "找回成功！");
    return true;
}

//挂失选项
int AdminDialog::L_loss(QString cardid)
{
    QSqlQuery query;
    QString str = QString("SELECT state FROM card WHERE cardid=%1").arg(cardid);
    if (!GetQuery(str, query))
        return -1;
    QString state = query.value(0).toString();
    if (state == "0")
    {
        QMessageBox::information(this, "提示", "您已经挂失过了！");
        return -2;
    }
    str = QString("SELECT is_using FROM record WHERE cardid=%1 AND is_using=1").arg(cardid);
    if (!query.exec(str))
    {
        qDebug("查询上机状态失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return -1;
    }
    if (query.next())
    {
        QMessageBox::information(this, "提示", "上机状态下不能挂失！");
        return -3;
    }
    if (QMessageBox::question(this, "挂失", "你是否确定要挂失？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return -4;
    QDateTime current_time = QDateTime::currentDateTime();
    QString losstime = current_time.toString("yyyy-MM-ddThh:mm:ss");
    str = QString("INSERT INTO loss_record "
                  "VALUES('%1','%2','%3','0')")
              .arg(losstime)
              .arg(cardid)
              .arg(admin_id);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "挂失失败！");
        qDebug() << query.lastError();
        return -1;
    }
    QMessageBox::information(this, "提示", "挂失成功！");
    return 0;
}

//点击充值按钮
void AdminDialog::on_c_Btn_clicked()
{
    if (QMessageBox::question(this, "充值", "你是否确定要充值？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;
    int money = ui->c_numSBox->value();
    //获取操作对象
    QString user = ui->c_userCBox->currentText();
    user = user.split('(').at(0);
    QString str = QString("SELECT cardid FROM card WHERE sno='%1'").arg(user);
    QSqlQuery query;
    if (!GetQuery(str, query))
        return;
    QString cardid = query.value(0).toString();
    QDateTime current_time = QDateTime::currentDateTime();
    QString ctime = current_time.toString("yyyy-MM-ddThh:mm:ss");
    str = QString("INSERT INTO charge_record(ctime,cardid,money,opeartor_id,state)"
                  " VALUES('%1','%2',%3,'%4',1)")
              .arg(ctime)
              .arg(cardid)
              .arg(money)
              .arg(admin_id);
    if (!query.exec(str))
    {
        QMessageBox::warning(this, "错误", "充值失败！");
        qDebug() << query.lastError();
        return;
    }
    QMessageBox::information(this, "提示", "充值成功！");
}

//全部日期复选框状态改变
void AdminDialog::on_i_checkBox_stateChanged(int arg1)
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

//执行SQL语句
bool AdminDialog::GetQuery(QString &str, QSqlQuery &query)
{
    if (!query.exec(str) || !query.next())
    {
        qDebug("查询失败！");
        qDebug() << str;
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

//点击学生信息查询按钮
void AdminDialog::on_m_QueryBtn_clicked()
{
    QSqlQuery query;
    QString str("SELECT sno,sname,sdc,sclass,sage,ssex FROM student");
    GetQuery(str, query);
    QStringList list = {"学号", "姓名", "系别", "班级", "年龄", "性别"};
    SetModel(query, list);
}

//点击学生管理按钮
void AdminDialog::on_m_addBtn_clicked()
{
    if (userdialog == nullptr)
    {
        userdialog = new UserChangeDialog(admin_id, this);
        connect(userdialog, SIGNAL(windowClosed()), this, SLOT(DialogClosed()));
        userdialog->show();
    }
}

//查询窗口关闭时
void AdminDialog::DialogClosed()
{
    delete userdialog;
    userdialog = nullptr;
    SetupName();
}
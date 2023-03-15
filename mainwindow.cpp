#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::openTable()
{
    //打开数据表
    qryModel = new QSqlQueryModel(this);
    theSelect = new QItemSelectionModel(qryModel);

    qryModel->setQuery("SELECT empNo, Name, Gender, Height, Birthday, Mobile, Province, City, Department, "
                       " Education, Salary FROM employee order by empNo"); //使用select语句查询数据
    if (qryModel->lastError().isValid())
    {
        QMessageBox::warning(this, "错误", "打开数据表失败" + qryModel->lastError().text(),
                             QMessageBox::Ok | QMessageBox::NoButton);
        return;
    }
    else
    {//设置模型表头文字
        qryModel->setHeaderData(0, Qt::Horizontal, "工号");
        qryModel->setHeaderData(1,Qt::Horizontal,"姓名");
        qryModel->setHeaderData(2,Qt::Horizontal,"性别");
        qryModel->setHeaderData(3,Qt::Horizontal,"身高");
        qryModel->setHeaderData(4,Qt::Horizontal,"出生日期");
        qryModel->setHeaderData(5,Qt::Horizontal,"手机");
        qryModel->setHeaderData(6,Qt::Horizontal,"省份");
        qryModel->setHeaderData(7,Qt::Horizontal,"城市");
        qryModel->setHeaderData(8,Qt::Horizontal,"部门");
        qryModel->setHeaderData(9,Qt::Horizontal,"学历");
        qryModel->setHeaderData(10,Qt::Horizontal,"工资");


        ui->tableView->setModel(qryModel);
        ui->tableView->setSelectionModel(theSelect);

        ui->actOpenDB->setEnabled(false);
        ui->actRecInsert->setEnabled(true);
        ui->actRecDelete->setEnabled(true);
        ui->actRecEdit->setEnabled(true);
        ui->actScan->setEnabled(true);

    }

}

void MainWindow::upDataRecord(int RowNo)
{//更新记录
    QSqlRecord curRecord = qryModel->record(RowNo); //获取模型当前记录模型
    int empNo = curRecord.value("EmpNO").toInt(); //当前员工工号

    QSqlQuery query;
    query.prepare("SELECT * from employee where EmpNo = :ID");
    query.bindValue(":ID", empNo); //将:ID用empNo代替
    query.exec(); //执行select语句 数据库数据中
    query.first(); //检索结果中的第一条记录（如果可用），并将查询定位在检索到的记录上。

    if (!query.isValid()) //是否为有效记录
    {
        return;
    }

    curRecord = query.record(); //当前记录
    WDialogData* dataDialog = new WDialogData(this);//创建对话框
    dataDialog->setUpdataRecord(curRecord);
    int rec = dataDialog->exec();// 以模态方式显示对话框
    if (rec == QDialog::Accepted)
    {
        QSqlRecord recData = dataDialog->getRecordData(); // /获得对话框返回的记录
        query.prepare("update employee set Name=:Name, Gender=:Gender,Height=:Height,"
                      " Birthday=:Birthday, Mobile=:Mobile, Province=:Province,"
                      " City=:City, Department=:Department, Education=:Education,"
                      " Salary=:Salary, Memo=:Memo, Photo=:Photo "
                      " where EmpNo = :ID");

        query.bindValue(":Name",recData.value("Name"));
        query.bindValue(":Gender",recData.value("Gender"));
        query.bindValue(":Height",recData.value("Height"));
        query.bindValue(":Birthday",recData.value("Birthday")); //sql用失败 在WDialogdata中禁止
        query.bindValue(":Mobile",recData.value("Mobile"));

        query.bindValue(":Province",recData.value("Province"));
        query.bindValue(":City",recData.value("City"));
        query.bindValue(":Department",recData.value("Department"));
        query.bindValue(":Education",recData.value("Education"));

        query.bindValue(":Salary",recData.value("Salary"));
        query.bindValue(":Memo",recData.value("Memo"));
        query.bindValue(":Photo",recData.value("Photo"));

        query.bindValue(":ID",empNo);


        if (!query.exec()) //打开错误
        {
            QMessageBox::warning(this, "错误", "更新数据错误" + query.lastError().text(),
                                 QMessageBox::Ok | QMessageBox::NoButton);
        }
        else
        {
            qryModel->query().exec(); //数据模型重新查询数据，更新tableView显示
        }
    }
    delete dataDialog;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); //选择行
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actOpenDB_triggered()
{
//    QString aFileName = QFileDialog::getOpenFileName(this, "选择数据库文件", "",
//                                                     "SQLINTE(*.db *db3)");
//    //QSQLINTE数据库
//    DB = QSqlDatabase::addDatabase("QSQLITE");
//    DB.setDatabaseName(aFileName);

    DB = QSqlDatabase::addDatabase("QODBC"); //数据库模块
    QString dsn = QString::fromLocal8Bit("QtSample"); //数据库
    DB.setHostName("localhost");
    DB.setDatabaseName(dsn);
    DB.setUserName("v");
    DB.setPassword("123456");

    if (!DB.open())
    {
        QMessageBox::warning(this, "错误", "打开数据库错误" + DB.lastError().text(),
                             QMessageBox::Ok | QMessageBox::NoButton);
        return;
    }
    openTable();
}

void MainWindow::on_actRecInsert_triggered()
{
    //插入记录
    QSqlQuery query; //QSqlQuery类提供了一种执行和操作的方法
    query.exec("select * from employee where EmpNO = -1"); //实际不查询出记录，只查询字段信息

    QSqlRecord curRecord = query.record(); //获取当前记录,实际为空记录
    //curRecord.setValue("EmpNo", qryModel->rowCount() + 3000);
    curRecord.setValue("EmpNo", qryModel->rowCount() + 3000);

    WDialogData* dataDialog = new WDialogData(this);
    dataDialog->setInsertRecord(curRecord);
    int ret = dataDialog->exec();
    if (ret == QDialog::Accepted)
    {
        QSqlRecord recData = dataDialog->getRecordData();
        query.prepare("INSERT INTO employee (EmpNo,Name,Gender,Height,Birthday,Mobile,Province,"
                      " City,Department,Education,Salary,Memo,Photo) "
                      " VALUES(:EmpNo,:Name, :Gender,:Height,:Birthday,:Mobile,:Province,"
                      " :City,:Department,:Education,:Salary,:Memo,:Photo)");

        query.bindValue(":EmpNo",recData.value("EmpNo"));
        query.bindValue(":Name",recData.value("Name"));
        query.bindValue(":Gender",recData.value("Gender"));
        query.bindValue(":Height",recData.value("Height"));
        query.bindValue(":Birthday",recData.value("Birthday"));
        query.bindValue(":Mobile",recData.value("Mobile"));

        query.bindValue(":Province",recData.value("Province"));
        query.bindValue(":City",recData.value("City"));
        query.bindValue(":Department",recData.value("Department"));
        query.bindValue(":Education",recData.value("Education"));

        query.bindValue(":Salary",recData.value("Salary"));
        query.bindValue(":Memo",recData.value("Memo"));
        query.bindValue(":Photo",recData.value("Photo"));

        if (!query.exec())
            QMessageBox::warning(this, "错误", "插入数据失败" + query.lastError().text(),
                                 QMessageBox::Ok | QMessageBox::NoButton);
        else //插入，删除记录后需要重新设置SQL语句查询
        {
            QString str = qryModel->query().executedQuery(); //  执行过的SELECT语句
            qryModel->setQuery(str);//重新查询数据
        }
    }
}

void MainWindow::on_actRecEdit_triggered()
{
    int curRow = theSelect->currentIndex().row();
    upDataRecord(curRow);
}

void MainWindow::on_actRecDelete_triggered()
{
    //删除当前记录
    int row = theSelect->currentIndex().row();
    QSqlRecord curRecord = qryModel->record(row);

    if (curRecord.isEmpty()) //空记录
        return;

    int empNo = curRecord.value("EmpNo").toInt();
    QSqlQuery query;
    query.prepare("delete from employee where EmpNo = :ID");
    query.bindValue(":ID",empNo);

    if (!query.exec())
        QMessageBox::warning(this, "错误", "删除错误" + query.lastError().text(),
                             QMessageBox::Ok | QMessageBox::NoButton);
    else
    {

        //插入，删除记录后需要重新设置SQL语句查询
        QString qur = qryModel->query().executedQuery();//  执行过的SELECT语句
        qryModel->setQuery(qur); //再执行一次
    }

}

void MainWindow::on_actScan_triggered()
{
    QSqlQuery qryEmpList; //员工工资信息列表
    qryEmpList.exec("SELECT empNo,Salary FROM employee ORDER BY empNo");
    qryEmpList.first();

    QSqlQuery qryUpdate; //临时 QSqlQuery
    qryUpdate.prepare("UPDATE employee SET Salary=:Salary WHERE EmpNo = :ID");

    while (qryEmpList.isValid()) //当前记录有效
    {
        int empID=qryEmpList.value("empNo").toInt(); //获取empNo
        float salary=qryEmpList.value("Salary").toFloat(); //获取Salary
        salary=salary+1000; //涨工资

        qryUpdate.bindValue(":ID",empID);
        qryUpdate.bindValue(":Salary",salary); //设置SQL语句参数
        qryUpdate.exec(); //执行update

        if (!qryEmpList.next()) //移动到下一条记录，并判断是否到末尾了
            break;
    }

    qryModel->query().exec();//数据模型重新查询数据，更新tableView的显示
    QMessageBox::information(this, "提示", "涨工资计算完毕",
                             QMessageBox::Ok,QMessageBox::NoButton);

}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QItemSelectionModel>
#include <QLabel>
#include <QDataWidgetMapper> //关系映射
#include <QFileDialog>
#include <QMessageBox>

#include "wdialogdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QSqlDatabase DB; //数据库
    QSqlQueryModel* qryModel; //数据模型
    QDataWidgetMapper* dataMapper; //组件与模型的映射
    QItemSelectionModel* theSelect; //选择模型

    void openTable(); //打开数据表
    void upDataRecord(int RowNo); //更新记录


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actOpenDB_triggered();

    void on_actRecInsert_triggered();

    void on_actRecEdit_triggered();

    void on_actRecDelete_triggered();

    void on_actScan_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

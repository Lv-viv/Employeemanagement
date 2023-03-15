#ifndef WDIALOGDATA_H
#define WDIALOGDATA_H

#include <QDialog>
#include <QSqlRecord>
#include <QFileDialog>
namespace Ui {
class WDialogData;
}

class WDialogData : public QDialog
{
    Q_OBJECT

private:
    QSqlRecord myRecord; //保存一条记录的数据


public:
    explicit WDialogData(QWidget *parent = 0);
    ~WDialogData();

    void setUpdataRecord(QSqlRecord& recData); //更新数据
    void setInsertRecord(QSqlRecord& Record); //插入数据

    QSqlRecord getRecordData(); // //获取录入的数据
private slots:
    void on_btnSetPhoto_clicked();

    void on_btnClearPhoto_clicked();

private:
    Ui::WDialogData *ui;
};

#endif // WDIALOGDATA_H

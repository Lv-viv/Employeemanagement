#include "wdialogdata.h"
#include "ui_wdialogdata.h"

WDialogData::WDialogData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WDialogData)
{
    ui->setupUi(this);
}

WDialogData::~WDialogData()
{
    delete ui;
}

void WDialogData::setUpdataRecord(QSqlRecord &recData)
{
    //编辑记录，更新记录数据到界面
    myRecord = recData;
    ui->spinEmpNo->setEnabled(false); //员工编号不允许编辑
    setWindowTitle("更新数据");

    ui->spinEmpNo->setValue(recData.value("empNo").toInt());
    ui->editName->setText(recData.value("Name").toString());
    ui->comboSex->setCurrentText(recData.value("Gender").toString());
    ui->spinHeight->setValue(recData.value("Height").toFloat());
    ui->editBirth->setDate(recData.value("Birthday").toDate());
    ui->editMobile->setText(recData.value("Mobile").toString());
    ui->comboProvince->setCurrentText(recData.value("Province").toString());
    ui->editCity->setText(recData.value("City").toString());
    ui->comboDep->setCurrentText(recData.value("Department").toString());
    ui->comboEdu->setCurrentText(recData.value("Education").toString());
    ui->spinSalary->setValue(recData.value("Salary").toInt());
    ui->editMemo->setPlainText(recData.value("Memo").toString());

    QVariant var = recData.value("Photo"); //返回当前记录中字段索引的值。 这些字段使用 SELECT 语句的文本从左到右编号，例如在
    if (!var.isValid())//图片字段内容为空
    {
        ui->LabPhoto->clear();
    }
    else
    {
        QByteArray byteArray = var.toByteArray();

        QPixmap pix;
        pix.loadFromData(byteArray);
        ui->LabPhoto->setPixmap(pix.scaledToWidth(ui->LabPhoto->size().width()));
    }
}

void WDialogData::setInsertRecord(QSqlRecord &Record)
{
    myRecord = Record;
    ui->spinEmpNo->setEnabled(true); //插入的记录，员工编号允许编辑
    setWindowTitle("更新数据");
    ui->spinEmpNo->setValue(Record.value("empNo").toInt());
}

QSqlRecord WDialogData::getRecordData()
{ //设置记录的数据
    myRecord.setValue("empNO", ui->spinEmpNo->value());
    myRecord.setValue("Name",ui->editName->text());
    myRecord.setValue("Gender",ui->comboSex->currentText());
    myRecord.setValue("Height",ui->spinHeight->value());
    //myRecord.setValue("Birthday",ui->editBirth->date()); sql失败
    myRecord.setValue("Birthday",ui->editBirth->dateTime());
    myRecord.setValue("Mobile",ui->editMobile->text());

    myRecord.setValue("Province",ui->comboProvince->currentText());
    myRecord.setValue("City",ui->editCity->text());
    myRecord.setValue("Department",ui->comboDep->currentText());

    myRecord.setValue("Education",ui->comboEdu->currentText());
    myRecord.setValue("Salary",ui->spinSalary->value());
    myRecord.setValue("Memo",ui->editMemo->toPlainText());

    //照片编辑时已经修改了mRecord的photo字段的值
    return myRecord;
}

void WDialogData::on_btnSetPhoto_clicked()
{
    //设置照片
    QString aFile=QFileDialog::getOpenFileName(this,"选择图片文件","",
                                               "照片(*.jpg)");
    if (aFile.isEmpty())
        return;

    QByteArray data;
    QFile* file=new QFile(aFile); //fileName为二进制数据文件名
    file->open(QIODevice::ReadOnly);
    data = file->readAll();
    file->close();

   // mRecord.setValue("Photo",data); //图片保存到Photo字段

    QPixmap pic;
    pic.loadFromData(data);
    ui->LabPhoto->setPixmap(pic.scaledToWidth(ui->LabPhoto->size().width()));
}



void WDialogData::on_btnClearPhoto_clicked()
{
    ui->LabPhoto->clear();
    myRecord.setNull("Photo");
}

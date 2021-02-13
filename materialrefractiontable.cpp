#include "materialrefractiontable.h"
#include "ui_materialrefractiontable.h"

#include <QFile>
#include <QTextStream>
#include <QSqlRecord>

MaterialRefractionTable::MaterialRefractionTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaterialRefractionTable)
{
    ui->setupUi(this);
}

MaterialRefractionTable::~MaterialRefractionTable()
{
    delete ui;
}

void MaterialRefractionTable::displayTable()
{
    model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable("material_refraction");
    model->select();

    ui->table->setModel(model);
    ui->table->resizeColumnsToContents();
    ui->table->resizeRowsToContents();
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MaterialRefractionTable::importTable(QString filename)
{
    QSqlRecord record = model->record();
    record.remove(record.indexOf("obj_id"));
    record.remove(record.indexOf("obj_uuid"));
    for (int i = 0; i < model->rowCount(); i++)
        model->removeRow(i);

    QFile file = QFile(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    }
    QTextStream in(&file);
    in.readLine();  // skip header
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList values = line.split(";");
        values.removeFirst();
        values.removeFirst();
        record.setValue("obj_name", values[0]);
        record.setValue("re_value_min", values[1]);
        record.setValue("im_value_min", values[2]);
        record.setValue("re_value_max", values[3]);
        record.setValue("im_value_max", values[4]);
        model->insertRecord(-1, record);
    }
    file.close();
    model->submitAll();
}

void MaterialRefractionTable::exportTable(QString filename)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream out(&file);
        QStringList list;
        int rows = model->rowCount();
        int cols = model->columnCount();
        for (int i = 0; i < cols; i++){
            list << model->headerData(i, Qt::Horizontal).toString();
        }
        out << list.join(";") << '\n';
        list.clear();

        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                list << model->data(model->index(i, j)).toString();
            }
            out << list.join(";") << '\n';
            list.clear();
        }
    }
    file.close();
}

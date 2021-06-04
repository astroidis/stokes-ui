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
    ui->table->hideColumn(model->fieldIndex("obj_id"));
    ui->table->hideColumn(model->fieldIndex("obj_uuid"));
}

#include "constantswindow.h"
#include "ui_constantswindow.h"


ConstantsWindow::ConstantsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConstantsWindow),
    model(new QSqlTableModel(this, QSqlDatabase::database("stokes_db")))
{
    ui->setupUi(this);
    setupTable();
}

ConstantsWindow::~ConstantsWindow()
{
    delete ui;
}

void ConstantsWindow::setupTable()
{
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("constants");
    model->select();

    ui->table->setModel(model);
    ui->table->setColumnWidth(model->fieldIndex("obj_descr"), 400);
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->hideColumn(0);
}


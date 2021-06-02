#include "experimentswindow.h"
#include "ui_experimentswindow.h"

#include <QSqlRecord>
#include <QDate>

#include "roweditingdelegate.h"
#include "newexperimentdialog.h"

ExperimentsWindow::ExperimentsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExperimentsWindow),
    modelExp(new QSqlTableModel(this, QSqlDatabase::database("stokes_db")))
{
    ui->setupUi(this);
    setupTable();
    setupToolbar();
}

ExperimentsWindow::~ExperimentsWindow()
{
    delete ui;
}

void ExperimentsWindow::setupToolbar()
{
    tb = new QToolBar(this);
    QAction *actSave = new QAction("Save", this);
    tb->addAction(actSave);
//    connect(actSave, &QAction::triggered, ...)

    QAction *actLoad = new QAction("Load", this);
    tb->addAction(actLoad);
//    connect(actLoad, &QAction::triggered, ...)

    QAction *actAdd = new QAction("Add", this);
    tb->addAction(actAdd);
    connect(actAdd, &QAction::triggered, this, &ExperimentsWindow::openAddDialog);

    QAction *actDelete = new QAction("Delete", this);
    tb->addAction(actDelete);
    connect(actDelete, &QAction::triggered, this, &ExperimentsWindow::deleteExperiment);

    QAction *actSelect = new QAction("Select", this);
    tb->addAction(actSelect);
    connect(actSelect, &QAction::triggered, this, &ExperimentsWindow::selectExperiment);

    ui->toolLayout->addWidget(tb);
}

void ExperimentsWindow::setupTable()
{
    modelExp->setTable("experiments");
    modelExp->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelExp->select();

    ui->exprimentsTable->setModel(modelExp);
    ui->exprimentsTable->hideColumn(modelExp->fieldIndex("obj_id"));
    ui->exprimentsTable->hideColumn(modelExp->fieldIndex("obj_uuid"));
    ui->exprimentsTable->setColumnWidth(modelExp->fieldIndex("obj_name"), 200);
    ui->exprimentsTable->setColumnWidth(modelExp->fieldIndex("obj_comment"), 400);
    ui->exprimentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->exprimentsTable->setItemDelegate(new RowEditingDelegate());
    ui->exprimentsTable->resizeRowsToContents();
}

void ExperimentsWindow::openAddDialog()
{
    NewExperimentDialog nedlg;

    if (nedlg.exec() == NewExperimentDialog::Accepted){
        QSqlRecord rec = modelExp->record();
        rec.remove(rec.indexOf("obj_id"));
        rec.remove(rec.indexOf("obj_uuid"));
        rec.setValue("obj_name", nedlg.new_name);
        rec.setValue("obj_comment", nedlg.new_comment);
        rec.setValue("date_created", QDate::currentDate());

        modelExp->insertRecord(-1, rec);
        modelExp->submitAll();
    }
}

void ExperimentsWindow::deleteExperiment()
{
    if (ui->exprimentsTable->selectionModel()->selectedRows().isEmpty()){
        return;
    }

    int row = ui->exprimentsTable->selectionModel()->selectedRows().back().row();
    modelExp->removeRow(row);
    modelExp->submitAll();
    modelExp->select();
    ui->exprimentsTable->reset();
}

void ExperimentsWindow::selectExperiment()
{
    if (ui->exprimentsTable->selectionModel()->selectedRows().isEmpty()){
        return;
    }

    int row = ui->exprimentsTable->selectionModel()->selectedRows().back().row();
    QSqlRecord rec = modelExp->record(row);

    emit experimentSelected(rec.value("obj_uuid").toString(), rec.value("obj_name").toString());
}

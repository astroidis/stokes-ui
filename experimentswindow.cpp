#include "experimentswindow.h"
#include "ui_experimentswindow.h"

#include <QSqlRecord>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

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
//    QAction *actSave = new QAction("Save", this);
//    tb->addAction(actSave);
//    connect(actSave, &QAction::triggered, ...)

//    QAction *actLoad = new QAction("Load", this);
//    tb->addAction(actLoad);
//    connect(actLoad, &QAction::triggered, ...)

    QAction *actAdd = new QAction("Создать", this);
    tb->addAction(actAdd);
    connect(actAdd, &QAction::triggered, this, &ExperimentsWindow::openAddDialog);

    QAction *actDelete = new QAction("Удалить", this);
    tb->addAction(actDelete);
    connect(actDelete, &QAction::triggered, this, &ExperimentsWindow::deleteExperiment);

    QAction *actSelect = new QAction("Выбрать", this);
    tb->addAction(actSelect);
    connect(actSelect, &QAction::triggered, this, &ExperimentsWindow::selectExperiment);

    ui->toolLayout->addWidget(tb);
}

void ExperimentsWindow::setupTable()
{
    modelExp->setTable("experiments");
    modelExp->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelExp->setSort(modelExp->fieldIndex("obj_id"), Qt::AscendingOrder);
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

void ExperimentsWindow::importRaysData(QString expuuid, QString filename)
{
    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setTable("calculation_12");
    QSqlRecord record = model->record();
    record.remove(model->fieldIndex("obj_id"));

    QFile file = QFile(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    }

    QTextStream in(&file);
    in.readLine();  // skip header
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList values = line.split(";");
        record.setValue("Alfa", values[0]);
        record.setValue("Beta", values[1]);
        record.setValue("Tau1", values[2]);
        record.setValue("Phi1", values[3]);
        record.setValue("I1", values[4]);
        record.setValue("Tau2", values[5]);
        record.setValue("Phi2", values[6]);
        record.setValue("I2", values[7]);
        record.setValue("Tau3", values[8]);
        record.setValue("Phi3", values[9]);
        record.setValue("I3", values[10]);
        record.setValue("Tau4", values[11]);
        record.setValue("Phi4", values[12]);
        record.setValue("I4", values[13]);
        record.setValue("experiment_id", expuuid);
        model->insertRecord(-1, record);
    }
    file.close();
    model->submitAll();
}

void ExperimentsWindow::importC1Data(QString expuuid, QString filename)
{
    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setTable("c1_parameter");
    QSqlRecord record = model->record();

    QFile file = QFile(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    }

    QTextStream in(&file);
    in.readLine();  // skip header
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList values = line.split(";");
        record.setValue("dTheta", values[0]);
        record.setValue("Q", values[1]);
        record.setValue("U", values[2]);
        record.setValue("V", values[3]);
        record.setValue("experiment_id", expuuid);
        model->insertRecord(-1, record);
    }
    file.close();
    model->submitAll();
}

void ExperimentsWindow::importC2Data(QString expuuid, QString filename)
{
    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setTable("c2_parameter");
    QSqlRecord record = model->record();

    QFile file = QFile(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    }

    QTextStream in(&file);
    in.readLine();  // skip header
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList values = line.split(";");
        record.setValue("dGamma", values[0]);
        record.setValue("Q", values[1]);
        record.setValue("U", values[2]);
        record.setValue("V", values[3]);
        record.setValue("experiment_id", expuuid);
        model->insertRecord(-1, record);
    }
    file.close();
    model->submitAll();
}

void ExperimentsWindow::importC3Data(QString expuuid, QString filename)
{
    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setTable("c3_parameter");
    QSqlRecord record = model->record();

    QFile file = QFile(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return;
    }

    QTextStream in(&file);
    in.readLine();  // skip header
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList values = line.split(";");
        record.setValue("dGamma", values[0]);
        record.setValue("Alpha", values[1]);
        record.setValue("Beta", values[2]);
        record.setValue("experiment_id", expuuid);
        model->insertRecord(-1, record);
    }
    file.close();
    model->submitAll();
}

void ExperimentsWindow::openAddDialog()
{
    NewExperimentDialog nedlg;
    connect(&nedlg, &NewExperimentDialog::importFiles, this, &ExperimentsWindow::importExperimentData);

    if (nedlg.exec() == NewExperimentDialog::Accepted){
        modelExp->select();
        ui->exprimentsTable->reset();
    }
}

void ExperimentsWindow::deleteExperiment()
{
    if (ui->exprimentsTable->selectionModel()->selectedRows().isEmpty()){
        return;
    }

    int ret = QMessageBox::warning(this, "Удаление", "Удалить эксперимент?",
                                   QMessageBox::Ok | QMessageBox::Cancel);

    if (ret == QMessageBox::Ok){
        int row = ui->exprimentsTable->selectionModel()->selectedRows().back().row();
        modelExp->removeRow(row);
        modelExp->submitAll();
        modelExp->select();
        ui->exprimentsTable->reset();
    }
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

void ExperimentsWindow::importExperimentData(QString expuuid, QString frays, QString fc1, QString fc2, QString fc3)
{
    importRaysData(expuuid, frays);
    importC1Data(expuuid, fc1);
    importC2Data(expuuid, fc2);
    importC3Data(expuuid, fc3);
}

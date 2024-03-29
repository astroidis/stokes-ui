#include "calc3window.h"
#include "ui_calc3window.h"

#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>

#include <map>
#include <string>

#include "calculations/task3.h"


Calc3Window::Calc3Window(QString experiment_id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc3Window),
    modelC1Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelC2Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelC3Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelStats(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    experiment(experiment_id),
    logger("statistics.log")
{
    ui->setupUi(this);
    setupToolbar();

    connect(ui->c1rbtn, &QRadioButton::toggled, this, &Calc3Window::c1rbtnClicked);
    connect(ui->c2rbtn, &QRadioButton::toggled, this, &Calc3Window::c2rbtnClicked);
    connect(ui->c3rbtn, &QRadioButton::toggled, this, &Calc3Window::c3rbtnClicked);
    connect(ui->calcrbtn, &QRadioButton::toggled, this, &Calc3Window::calcrbtnClicked);
    connect(ui->calcParamPbtn, &QPushButton::clicked, this, &Calc3Window::calcParamPbtnClicked);
    connect(ui->statpbtn, &QPushButton::clicked, this, &Calc3Window::calcStats);
}

Calc3Window::~Calc3Window()
{
    delete ui;
}

void Calc3Window::setupToolbar()
{
    tb = new QToolBar(this);
    QAction *saveAct = new QAction("Сохранить", this);
    tb->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &Calc3Window::saveData);

    QAction *loadAct = new QAction("Загрузить", this);
    tb->addAction(loadAct);
    connect(loadAct, &QAction::triggered, this, &Calc3Window::loadData);

    ui->toolLayout->addWidget(tb);
    tb->show();
}

void Calc3Window::calcC1()
{
    for (int i = 0; i < modelC1Calc->rowCount(); i++){
        QSqlRecord rec = modelC1Calc->record(i);
        Task3::ItemC1 item(rec.value("dTheta").toDouble(), rec.value("Q").toDouble(),
                           rec.value("U").toDouble(), rec.value("V").toDouble());
        rec.setValue("C1", QString::number(item.calc(), 'f', 4));
        rec.setValue("is_calc", item.iscalc);
        modelC1Calc->setRecord(i, rec);
    }
}

void Calc3Window::calcC2()
{
    for (int i = 0; i < modelC2Calc->rowCount(); i++){
        QSqlRecord rec = modelC2Calc->record(i);
        Task3::ItemC2 item(rec.value("dGamma").toDouble(), rec.value("Q").toDouble(),
                           rec.value("U").toDouble(), rec.value("V").toDouble());
        rec.setValue("C2", QString::number(item.calc(), 'f', 4));
        rec.setValue("is_calc", item.iscalc);
        modelC2Calc->setRecord(i, rec);
    }
}

void Calc3Window::calcC3()
{
    for (int i = 0; i < modelC3Calc->rowCount(); i++){
        QSqlRecord rec = modelC3Calc->record(i);
        Task3::ItemC3 item(rec.value("dGamma").toDouble(), rec.value("Alpha").toDouble(),
                           rec.value("Beta").toDouble());
        rec.setValue("C3", QString::number(item.calc(), 'f', 4));
        rec.setValue("is_calc", item.iscalc);
        modelC3Calc->setRecord(i, rec);
    }
}

void Calc3Window::c1rbtnClicked()
{
    modelC1Calc->setTable("c1_parameter");
    modelC1Calc->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelC1Calc->setSort(modelC1Calc->fieldIndex("dTheta"), Qt::AscendingOrder);
    modelC1Calc->setFilter(QString("experiment_id = '%1'").arg(experiment));
    modelC1Calc->select();

    ui->C1table->setModel(modelC1Calc);
    ui->C1table->hideColumn(modelC1Calc->fieldIndex("experiment_id"));
    ui->stackedWidget->setCurrentWidget(ui->pageC1);
}

void Calc3Window::c2rbtnClicked()
{
    modelC2Calc->setTable("c2_parameter");
    modelC2Calc->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelC2Calc->setSort(modelC2Calc->fieldIndex("dGamma"), Qt::AscendingOrder);
    modelC2Calc->setFilter(QString("experiment_id = '%1'").arg(experiment));
    modelC2Calc->select();

    ui->C2table->setModel(modelC2Calc);
    ui->C2table->hideColumn(modelC2Calc->fieldIndex("experiment_id"));
    ui->stackedWidget->setCurrentWidget(ui->pageC2);
}

void Calc3Window::c3rbtnClicked()
{
    modelC3Calc->setTable("c3_parameter");
    modelC3Calc->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelC3Calc->setSort(modelC3Calc->fieldIndex("dGamma"), Qt::AscendingOrder);
    modelC3Calc->setFilter(QString("experiment_id = '%1'").arg(experiment));
    modelC3Calc->select();

    ui->C3table->setModel(modelC3Calc);
    ui->C3table->hideColumn(modelC3Calc->fieldIndex("experiment_id"));
    ui->stackedWidget->setCurrentWidget(ui->pageC3);
}

void Calc3Window::calcrbtnClicked()
{
    modelStats->setTable("statistics");
    modelStats->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelStats->setFilter(QString("experiment_id = '%1'").arg(experiment));
    modelStats->setSort(modelStats->fieldIndex("obj_name"), Qt::AscendingOrder);
    modelStats->select();

    ui->statsTable->setModel(modelStats);
    ui->statsTable->hideColumn(0);
    ui->statsTable->hideColumn(modelStats->fieldIndex("experiment_id"));
    ui->statsTable->resizeColumnToContents(modelStats->fieldIndex("obj_name"));
    ui->stackedWidget->setCurrentWidget(ui->pageCalc);
}

void Calc3Window::calcParamPbtnClicked()
{
    if (ui->c1rbtn->isChecked()){
        qDebug() << "Calculating C1";
        calcC1();
    }
    else if (ui->c2rbtn->isChecked()){
        qDebug() << "Calculating C2";
        calcC2();
    }
    else if (ui->c3rbtn->isChecked()){
        qDebug() << "Calculating C3";
        calcC3();
    }
}

void Calc3Window::calcStats()
{
    QSqlTableModel model1 = QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model1.setTable("c1_parameter");
    model1.setFilter(QString("experiment_id = '%1'").arg(experiment));
    model1.select();

    QSqlTableModel model2 = QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model2.setTable("c2_parameter");
    model2.setFilter(QString("experiment_id = '%1'").arg(experiment));
    model2.select();

    QSqlTableModel model3 = QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model3.setTable("c3_parameter");
    model3.setFilter(QString("experiment_id = '%1'").arg(experiment));
    model3.select();

    std::vector<Task3::ItemC> items1;
    std::vector<Task3::ItemC> items2;
    std::vector<Task3::ItemC> items3;

    items1.reserve(model1.rowCount());
    items2.reserve(model2.rowCount());
    items3.reserve(model3.rowCount());

    logger.logInfo("Calculating stat C1");

    for (int i = 0; i < model1.rowCount(); i++){
        QSqlRecord rec = model1.record(i);
        Task3::ItemC1 a(rec.value("dTheta").toDouble(), rec.value("Q").toDouble(),
                        rec.value("U").toDouble(), rec.value("V").toDouble());
        a.value = rec.value("C1").toDouble();
        a.iscalc = rec.value("is_calc").toBool();
        items1.push_back(a);
    }

    auto stat1 = Task3::calcStat(items1);

    logger.logInfo("Calculating stat C2");

    for (int i = 0; i < model2.rowCount(); i++){
        QSqlRecord rec = model2.record(i);
        Task3::ItemC2 a(rec.value("dGamma").toDouble(), rec.value("Q").toDouble(),
                        rec.value("U").toDouble(), rec.value("V").toDouble());
        a.value = rec.value("C2").toDouble();
        a.iscalc = rec.value("is_calc").toBool();
        items2.push_back(a);
    }

    auto stat2 = Task3::calcStat(items2);

    logger.logInfo("Calculating stat C3");

    for (int i = 0; i < model3.rowCount(); i++){
        QSqlRecord rec = model3.record(i);
        Task3::ItemC3 a(rec.value("dGamma").toDouble(), rec.value("Alfa").toDouble(),
                        rec.value("Beta").toDouble());
        a.value = rec.value("C3").toDouble();
        a.iscalc = rec.value("is_calc").toBool();
        items3.push_back(a);
    }

    auto stat3 = Task3::calcStat(items3);

    for (int i = 0; i < modelStats->rowCount(); i++){
        QSqlRecord rec = modelStats->record(i);
        std::string s = rec.value("obj_name").toString().toStdString();
        rec.setValue("c1_value", QString::number(stat1[s], 'f', 4));
        rec.setValue("c2_value", QString::number(stat2[s], 'f', 4));
        rec.setValue("c3_value", QString::number(stat3[s], 'f', 4));
        modelStats->setRecord(i, rec);
    }
}

void Calc3Window::loadData()
{
    QSqlTableModel *model;
    QTableView *table;
    if (ui->c1rbtn->isChecked()){
        model = modelC1Calc;
        table = ui->C1table;
    }
    else if (ui->c2rbtn->isChecked()){
        model = modelC2Calc;
        table = ui->C2table;
    }
    else if (ui->c3rbtn->isChecked()){
        model = modelC3Calc;
        table = ui->C3table;
    }
    else if (ui->calcrbtn->isChecked()){
        model = modelStats;
        table = ui->statsTable;
    }

    QMessageBox msg(QMessageBox::Warning, "Warning", "Несохраненные данные будут удалены");
    msg.addButton("Продолжить", QMessageBox::AcceptRole);
    msg.addButton("Отмена", QMessageBox::RejectRole);
    switch (msg.exec()){
        case QMessageBox::AcceptRole:
            qDebug() << "Reloading data\n";
            model->select();
            table->reset();
            break;

        case QMessageBox::RejectRole:
            qDebug() << "Loading canceled\n";
            msg.close();
            break;

        default:
            qDebug() << "Closing dialog\n";
            msg.close();
            break;
    }
}

void Calc3Window::saveData()
{
    QSqlTableModel *model;
    if (ui->c1rbtn->isChecked()){
        model = modelC1Calc;
    }
    else if (ui->c2rbtn->isChecked()){
        model = modelC2Calc;
    }
    else if (ui->c3rbtn->isChecked()){
        model = modelC3Calc;
    }
    else if (ui->calcrbtn->isChecked()){
        model = modelStats;
    }

    model->submitAll();
}

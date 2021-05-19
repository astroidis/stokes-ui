#include "calc3window.h"
#include "ui_calc3window.h"

#include <QPushButton>
#include <QTreeWidgetItem>
#include <QSqlRecord>
#include <QDebug>
#include <map>
#include <string>
#include "calculations/task3.h"

Calc3Window::Calc3Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc3Window),
    modelC1Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelC2Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelC3Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelStats(new QSqlTableModel(this, QSqlDatabase::database("stokes_db")))
{
    ui->setupUi(this);

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

void Calc3Window::calcC1()
{
    for (int i = 0; i < modelC1Calc->rowCount(); i++){
        QSqlRecord rec = modelC1Calc->record(i);
        qDebug() << i;
        Task3::ItemC1 item(rec.value("dTheta").toDouble(), rec.value("Q").toDouble(),
                           rec.value("U").toDouble(), rec.value("V").toDouble());
        rec.setValue("C1", item.calc());
        modelC1Calc->setRecord(i, rec);
    }
}

void Calc3Window::calcC2()
{
    for (int i = 0; i < modelC2Calc->rowCount(); i++){
        QSqlRecord rec = modelC2Calc->record(i);
        Task3::ItemC2 item(rec.value("dGamma").toDouble(), rec.value("Q").toDouble(),
                           rec.value("U").toDouble(), rec.value("V").toDouble());
        rec.setValue("C2", item.calc());
        modelC2Calc->setRecord(i, rec);
    }
}

void Calc3Window::calcC3()
{
    for (int i = 0; i < modelC3Calc->rowCount(); i++){
        QSqlRecord rec = modelC3Calc->record(i);
        Task3::ItemC3 item(rec.value("dGamma").toDouble(), rec.value("Alfa").toDouble(),
                           rec.value("Beta").toDouble());
        rec.setValue("C3", item.calc());
        modelC3Calc->setRecord(i, rec);
    }
}

void Calc3Window::c1rbtnClicked()
{
    modelC1Calc->setTable("c1_parameter");
    modelC1Calc->select();
    modelC1Calc->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->C1table->setModel(modelC1Calc);
    ui->C1table->resizeColumnsToContents();
    ui->C1table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC1);
}

void Calc3Window::c2rbtnClicked()
{
    modelC2Calc->setTable("c2_parameter");
    modelC2Calc->select();
    modelC2Calc->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->C2table->setModel(modelC2Calc);
    ui->C2table->resizeColumnsToContents();
    ui->C2table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC2);
}

void Calc3Window::c3rbtnClicked()
{
    modelC3Calc->setTable("c3_parameter");
    modelC3Calc->select();
    modelC3Calc->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->C3table->setModel(modelC3Calc);
    ui->C3table->resizeColumnsToContents();
    ui->C3table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC3);
}

void Calc3Window::calcrbtnClicked()
{
    modelStats->setTable("statistics");
    modelStats->select();
    modelStats->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->statsTable->setModel(modelStats);
    ui->statsTable->hideColumn(0);
    ui->statsTable->resizeColumnsToContents();
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
    model1.select();
    model1.sort(0, Qt::AscendingOrder);
//    virtual void setSort(int column, Qt::SortOrder order);

    QSqlTableModel model2 = QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model2.setTable("c2_parameter");
    model2.select();

    QSqlTableModel model3 = QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model3.setTable("c3_parameter");
    model3.select();

    std::vector<Task3::ItemC> items1;
    std::vector<Task3::ItemC> items2;
    std::vector<Task3::ItemC> items3;

    items1.reserve(model1.rowCount());
    items2.reserve(model2.rowCount());
    items3.reserve(model3.rowCount());

    qDebug() << "Calculating stat C1";
    for (int i = 0; i < model1.rowCount(); i++){
        QSqlRecord rec = model1.record(i);
        Task3::ItemC1 a(rec.value("dTheta").toDouble(), rec.value("Q").toDouble(),
                        rec.value("U").toDouble(), rec.value("V").toDouble());
        a.value = rec.value("C1").toDouble();
        items1.push_back(a);
    }

    auto stat1 = Task3::calcStat(items1);

    qDebug() << "Calculating stat C2";
    for (int i = 0; i < model2.rowCount(); i++){
        QSqlRecord rec = model2.record(i);
        Task3::ItemC2 a(rec.value("dGamma").toDouble(), rec.value("Q").toDouble(),
                        rec.value("U").toDouble(), rec.value("V").toDouble());
        a.value = rec.value("C2").toDouble();
        items2.push_back(a);
    }

    auto stat2 = Task3::calcStat(items2);

    qDebug() << "Calculating stat C3";
    for (int i = 0; i < model3.rowCount(); i++){
        QSqlRecord rec = model3.record(i);
        Task3::ItemC3 a(rec.value("dGamma").toDouble(), rec.value("Alfa").toDouble(),
                        rec.value("Beta").toDouble());
        a.value = rec.value("C3").toDouble();
        items3.push_back(a);
    }

    auto stat3 = Task3::calcStat(items3);

    for (int i = 0; i < modelStats->rowCount(); i++){
        QSqlRecord rec = modelStats->record(i);
        std::string s = rec.value("obj_name").toString().toStdString();
        rec.setValue("c1_value", QString::number(stat1[s], 'f', 5));
        rec.setValue("c2_value", QString::number(stat2[s], 'f', 5));
        rec.setValue("c3_value", QString::number(stat3[s], 'f', 5));
        qDebug() << s.data() << stat1[s] << stat2[s] << stat3[s] << "\n";
        modelStats->setRecord(i, rec);
    }
}

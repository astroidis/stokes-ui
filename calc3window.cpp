#include "calc3window.h"
#include "ui_calc3window.h"

#include <QPushButton>
#include <QTreeWidgetItem>
#include <QSqlRecord>
#include <QDebug>
#include "calculations/task3.h"

Calc3Window::Calc3Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc3Window),
    modelC1Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelC2Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelC3Calc(new QSqlTableModel(this, QSqlDatabase::database("stokes_db"))),
    modelC1Stat(new QSqlQueryModel(this)),
    modelC2Stat(new QSqlQueryModel(this)),
    modelC3Stat(new QSqlQueryModel(this)),
    tableC1Stat(new QTableView(this)),
    tableC2Stat(new QTableView(this)),
    tableC3Stat(new QTableView(this)),
    c1Item(new QTreeWidgetItem()),
    c2Item(new QTreeWidgetItem()),
    c3Item(new QTreeWidgetItem()),
    c1Child(new QTreeWidgetItem()),
    c2Child(new QTreeWidgetItem()),
    c3Child(new QTreeWidgetItem())
{
    ui->setupUi(this);
//    ui->stackedWidget->setCurrentWidget(ui->emptyPage);

    connect(ui->c1rbtn, &QRadioButton::toggled, this, &Calc3Window::c1rbtnClicked);
    connect(ui->c2rbtn, &QRadioButton::toggled, this, &Calc3Window::c2rbtnClicked);
    connect(ui->c3rbtn, &QRadioButton::toggled, this, &Calc3Window::c3rbtnClicked);
    connect(ui->calcrbtn, &QRadioButton::toggled, this, &Calc3Window::calcrbtnClicked);
    connect(ui->calcParamPbtn, &QPushButton::clicked, this, &Calc3Window::calcParamPbtnClicked);
}

Calc3Window::~Calc3Window()
{
    delete ui;
}

void Calc3Window::calcC1()
{
    for (int i = 0; i < modelC1Calc->rowCount(); i++){
        QSqlRecord rec = modelC1Calc->record(i);
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

    ui->C1table->setModel(modelC1Calc);
    ui->C1table->resizeColumnsToContents();
    ui->C1table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC1);
}

void Calc3Window::c2rbtnClicked()
{
    modelC2Calc->setTable("c2_parameter");
    modelC2Calc->select();

    ui->C2table->setModel(modelC2Calc);
    ui->C2table->resizeColumnsToContents();
    ui->C2table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC2);
}

void Calc3Window::c3rbtnClicked()
{
    modelC3Calc->setTable("c3_parameter");
    modelC3Calc->select();

    ui->C3table->setModel(modelC3Calc);
    ui->C3table->resizeColumnsToContents();
    ui->C3table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC3);
}

void Calc3Window::calcrbtnClicked()
{
    c1Item->setText(0, "Признак С1");
    c2Item->setText(0, "Признак С2");
    c3Item->setText(0, "Признак С3");
    ui->calcWidget->addTopLevelItem(c1Item);
    ui->calcWidget->addTopLevelItem(c2Item);
    ui->calcWidget->addTopLevelItem(c3Item);

    c1Item->addChild(c1Child);
    c2Item->addChild(c2Child);
    c3Item->addChild(c3Child);

    modelC1Stat->setQuery("select obj_id, obj_name, c1_value from statistics",
                          QSqlDatabase::database("stokes_db"));
    modelC2Stat->setQuery("select obj_id, obj_name, c2_value from statistics",
                          QSqlDatabase::database("stokes_db"));
    modelC3Stat->setQuery("select obj_id, obj_name, c3_value from statistics",
                          QSqlDatabase::database("stokes_db"));

    tableC1Stat->setModel(modelC1Stat);
    tableC2Stat->setModel(modelC2Stat);
    tableC3Stat->setModel(modelC3Stat);

    ui->calcWidget->setItemWidget(c1Child, 0, tableC1Stat);
    ui->calcWidget->setItemWidget(c2Child, 0, tableC2Stat);
    ui->calcWidget->setItemWidget(c3Child, 0, tableC3Stat);


    ui->stackedWidget->setCurrentWidget(ui->pageCalc);
}

void Calc3Window::calcParamPbtnClicked()
{
    if (ui->c1rbtn->isChecked()){
        calcC1();
    }
    else if (ui->c2rbtn->isChecked()){
        calcC2();
    }
    else if (ui->c3rbtn->isChecked()){
        calcC3();
    }
}

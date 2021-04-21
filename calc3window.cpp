#include "calc3window.h"
#include "ui_calc3window.h"

#include <QPushButton>
#include <QTreeWidgetItem>


Calc3Window::Calc3Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc3Window),
    c1Calculated(new QTableView()),
    c2Calculated(new QTableView()),
    c3Calculated(new QTableView())
{
    ui->setupUi(this);
//    ui->stackedWidget->setCurrentWidget(ui->emptyPage);

    connect(ui->c1rbtn, &QRadioButton::toggled, this, &Calc3Window::c1rbtnClicked);
    connect(ui->c2rbtn, &QRadioButton::toggled, this, &Calc3Window::c2rbtnClicked);
    connect(ui->c3rbtn, &QRadioButton::toggled, this, &Calc3Window::c3rbtnClicked);
    connect(ui->calcrbtn, &QRadioButton::toggled, this, &Calc3Window::calcrbtnClicked);
}

Calc3Window::~Calc3Window()
{
    delete ui;
}

void Calc3Window::c1rbtnClicked()
{
    QSqlTableModel *modelC1 = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    modelC1->setTable("c1_parameter");
    modelC1->select();

    ui->C1table->setModel(modelC1);
    ui->C1table->resizeColumnsToContents();
    ui->C1table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC1);
}

void Calc3Window::c2rbtnClicked()
{
    QSqlTableModel *modelC2 = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    modelC2->setTable("c2_parameter");
    modelC2->select();

    ui->C2table->setModel(modelC2);
    ui->C2table->resizeColumnsToContents();
    ui->C2table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC2);
}

void Calc3Window::c3rbtnClicked()
{
    QSqlTableModel *modelC3 = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    modelC3->setTable("c3_parameter");
    modelC3->select();

    ui->C3table->setModel(&modelC3);
    ui->C3table->resizeColumnsToContents();
    ui->C3table->resizeRowsToContents();

    ui->stackedWidget->setCurrentWidget(ui->pageC3);
}

void Calc3Window::calcrbtnClicked()
{
    QTreeWidgetItem *c1Item = new QTreeWidgetItem();
    QTreeWidgetItem *c2Item = new QTreeWidgetItem();
    QTreeWidgetItem *c3Item = new QTreeWidgetItem();
    c1Item->setText(0, "Признак С1");
    c2Item->setText(0, "Признак С2");
    c3Item->setText(0, "Признак С3");
    ui->calcWidget->addTopLevelItem(c1Item);
    ui->calcWidget->addTopLevelItem(c2Item);
    ui->calcWidget->addTopLevelItem(c3Item);

    QTreeWidgetItem *c1Child = new QTreeWidgetItem();
    QTreeWidgetItem *c2Child = new QTreeWidgetItem();
    QTreeWidgetItem *c3Child = new QTreeWidgetItem();
    c1Item->addChild(c1Child);
    c2Item->addChild(c2Child);
    c3Item->addChild(c3Child);
    ui->calcWidget->setItemWidget(c1Child, 0, c1Calculated);
    ui->calcWidget->setItemWidget(c2Child, 0, c2Calculated);
    ui->calcWidget->setItemWidget(c3Child, 0, c3Calculated);

    ui->stackedWidget->setCurrentWidget(ui->pageCalc);
}

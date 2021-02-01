#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(new Database())
{
    ui->setupUi(this);
    db->makeConnection();
    db->openDB();
    setupToolbar();

    connect(ui->actionLoadData12, &QAction::triggered, this, &MainWindow::loadTable);
}

MainWindow::~MainWindow()
{
    db->closeDB();
    delete ui;
}

void MainWindow::setupToolbar()
{
    QToolButton *button_load = new QToolButton();
    button_load->setPopupMode(QToolButton::MenuButtonPopup);
    button_load->addAction(ui->actionLoadData12);
    button_load->addAction(ui->actionLoadData3);
    button_load->setText("Load");
    ui->toolBar->addWidget(button_load);

    QToolButton *button_calc = new QToolButton();
    button_calc->setPopupMode(QToolButton::MenuButtonPopup);
    button_load->addAction(ui->actionCalculate12);
    button_calc->addAction(ui->actionCalculate3);
    button_calc->setText("Calculate");
    ui->toolBar->addWidget(button_calc);

    QToolButton *button_plot = new QToolButton();
    button_plot->setText("Plot");
    ui->toolBar->addWidget(button_plot);

    QToolButton *button_print = new QToolButton();
    button_print->setPopupMode(QToolButton::MenuButtonPopup);
    button_print->addAction(ui->actionCalculationLog);
    button_print->addAction(ui->actionDispersedRays);
    button_print->addAction(ui->actionNaturalRays);
    button_print->addAction(ui->actionPolarization);
    button_print->setText("Print");
    ui->toolBar->addWidget(button_print);
}

void MainWindow::loadTable()
{
    table = new Spreadsheet();
    table->displayTable("datatest_02");
    ui->mdiArea->addSubWindow(table);
    table->show();
}

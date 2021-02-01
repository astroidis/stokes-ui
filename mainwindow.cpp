#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QTableWidget>

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
    connect(ui->actionParameters, &QAction::triggered, this, &MainWindow::createParameterTable);
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
    table->setMinimumHeight(400);
    table->setMinimumWidth(400);
    ui->mdiArea->addSubWindow(table);
    table->show();
}

void MainWindow::createParameterTable()
{
    QTableWidget *table = new QTableWidget(4, 3);
    QStringList header;
    header << "Параметр" << "Описание параметра" << "Значение";
    table->setHorizontalHeaderLabels(header);

    table->setItem(0, 0, new QTableWidgetItem("Re(Nju)"));
    table->setItem(0, 1, new QTableWidgetItem("Действительная часть показателя преломления"));
    table->setItem(0, 2, new QTableWidgetItem(QString("%1").arg(1.4, 0, 'f')));

    table->setItem(1, 0, new QTableWidgetItem("Im(Nju)"));
    table->setItem(1, 1, new QTableWidgetItem("Мнимая часть показателя преломления"));
    table->setItem(1, 2, new QTableWidgetItem(QString("%1").arg(-4.53, 0, 'f')));

    table->setItem(2, 0, new QTableWidgetItem("Gamma"));
    table->setItem(2, 1, new QTableWidgetItem("Угол, град"));
    table->setItem(2, 2, new QTableWidgetItem(QString("%1").arg(45.00005, 0, 'f')));

    table->setItem(3, 0, new QTableWidgetItem("Phi"));
    table->setItem(3, 1, new QTableWidgetItem("Угол падения, град"));
    table->setItem(3, 2, new QTableWidgetItem(QString("%1").arg(52.0, 0, 'f')));

    table->resizeColumnsToContents();

    QWidget *w = new QWidget();
    w->setLayout(new QHBoxLayout());
    w->layout()->addWidget(table);
    w->setMinimumHeight(400);
    w->setMinimumWidth(400);

    ui->mdiArea->addSubWindow(w);
    w->show();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QFileDialog>
#include <QPushButton>

#include "experimentswindow.h"
#include "newexperimentdialog.h"
#include "materialrefractiontable.h"
#include "calc12window.h"
#include "calc3window.h"
#include "plotwidget.h"
#include "constantswindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = Database::instance();
    db->makeConnection();
    db->openDB();

    defaultTitle = this->windowTitle();

    connect(ui->actionParameters, &QAction::triggered, this, &MainWindow::openConstants);
    connect(ui->actionPlot, &QAction::triggered, this, &MainWindow::loadPlot);
    connect(ui->actionExperiments, &QAction::triggered, this, &MainWindow::openAllExperiments);
    connect(ui->actionRefraction, &QAction::triggered, this, &MainWindow::openRefraction);
    connect(ui->actionCalculate12, &QAction::triggered, this, &MainWindow::makeCalc12);
    connect(ui->actionCalculate3, &QAction::triggered, this, &MainWindow::makeCalc3);
}

MainWindow::~MainWindow()
{
    db->closeDB();
    delete ui;
}

void MainWindow::openConstants()
{
    ConstantsWindow *cw = new ConstantsWindow();
    ui->mdiArea->addSubWindow(cw);
    cw->showMaximized();
}

void MainWindow::loadPlot()
{
    PlotWidget *pw = new PlotWidget(selectedExperimentUUID);
    ui->mdiArea->addSubWindow(pw);
    pw->showMaximized();
}

void MainWindow::openAllExperiments()
{
    ExperimentsWindow *ew = new ExperimentsWindow();
    ui->mdiArea->addSubWindow(ew);
    ew->showMaximized();
    connect(ew, &ExperimentsWindow::experimentSelected, this, &MainWindow::currentExperiment);
    connect(ew, SIGNAL(experimentSelected(QString, QString)), this, SLOT(setMenuEnabled()));
}

void MainWindow::setMenuEnabled()
{
    ui->menuCalculate->setEnabled(true);
    ui->menuPrint->setEnabled(true);
    ui->menuTesting->setEnabled(true);
}

void MainWindow::currentExperiment(QString uuid, QString name)
{
    this->setWindowTitle(defaultTitle + " - " + name);
    this->selectedExperimentUUID = uuid;
}

void MainWindow::openRefraction()
{
    MaterialRefractionTable *table = new MaterialRefractionTable();
    ui->mdiArea->addSubWindow(table);
    table->showMaximized();
    table->displayTable();
}

void MainWindow::makeCalc12()
{
    Calc12Window *calc = new Calc12Window(selectedExperimentUUID);
    ui->mdiArea->addSubWindow(calc);
    calc->showMaximized();
}

void MainWindow::makeCalc3()
{
    Calc3Window *calc = new Calc3Window(selectedExperimentUUID);
    ui->mdiArea->addSubWindow(calc);
    calc->showMaximized();
}

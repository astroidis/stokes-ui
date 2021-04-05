#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QTableWidget>
#include <QLabel>
#include <QImage>
#include <QProcess>
#include <QFileDialog>

#include "newexperimentdialog.h"
#include "experimentselectordialog.h"
#include "materialrefractiontable.h"
#include "calc12window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = Database::instance();
    db->makeConnection();
    db->openDB();
    setupToolbar();

    connect(ui->actionLoadData12, &QAction::triggered, this, &MainWindow::loadTable);
    connect(ui->actionParameters, &QAction::triggered, this, &MainWindow::createParameterTable);
    connect(ui->actionPlot, &QAction::triggered, this, &MainWindow::loadPlot);
    connect(ui->actionCalculate12, &QAction::triggered, this, &MainWindow::runPython);
    connect(ui->actionNewExperiment, &QAction::triggered, this, &MainWindow::addNewExperiment);
    connect(ui->actionOpenExperiment, &QAction::triggered, this, &MainWindow::openAllExperiments);
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::openExperimentSelector);
    connect(ui->actionOpenRefraction_2, &QAction::triggered, this, &MainWindow::openRefraction);
    connect(ui->actionExportRefraction_2, &QAction::triggered, this, &MainWindow::exportRefraction);
    connect(ui->actionImportRefraction_2, &QAction::triggered, this, &MainWindow::importRefraction);
    connect(ui->actionCalculate12, &QAction::triggered, this, &MainWindow::makeCalc12);
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
    button_calc->addAction(ui->actionCalculate12);
    button_calc->addAction(ui->actionCalculate3);
    button_calc->setText("Calculate");
    ui->toolBar->addWidget(button_calc);

    QToolButton *button_plot = new QToolButton();
    button_plot->setText("Plot");
    button_plot->setDefaultAction(ui->actionPlot);
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

void MainWindow::loadPlot()
{
    QWidget *w = new QWidget();
    QHBoxLayout *l = new QHBoxLayout();
    w->setLayout(l);

    QImage img = QImage("C:\\Users\\User\\Documents\\Code\\Cpp\\TestPyEmbed\\ebosh.jpg");
    QLabel *lbl = new QLabel();
    lbl->setPixmap(QPixmap::fromImage(img.scaled(600, 400, Qt::KeepAspectRatio)));

    w->layout()->addWidget(lbl);
    ui->mdiArea->addSubWindow(w);
    w->show();
}

void MainWindow::runPython()
{
    QProcess pyproc;
    QStringList args;
    args << "C:\\Users\\User\\Documents\\Code\\sketch\\sample_plots.py";
    QString prog = "C:\\Users\\User\\Documents\\Code\\sketch\\Scripts\\python.exe";
    pyproc.startDetached(prog, args);
}

void MainWindow::addNewExperiment()
{
    NewExperimentDialog dialog(this);
    dialog.exec();
}

void MainWindow::openAllExperiments()
{
    experiments = new Spreadsheet();
    experiments->displayTable("experiments");
    ui->mdiArea->addSubWindow(experiments);
    experiments->show();
}

void MainWindow::openExperimentSelector()
{
    ExperimentSelectorDialog *dialog = new ExperimentSelectorDialog();
    connect(dialog, &ExperimentSelectorDialog::experimentSelected, this, &MainWindow::currentExperiment);
    dialog->exec();
}

void MainWindow::currentExperiment(QString uuid, QString name)
{
    this->setWindowTitle(this->windowTitle() + " - " + name);
    this->selectedExperiment = uuid;
}

void MainWindow::openRefraction()
{
    MaterialRefractionTable *table = new MaterialRefractionTable();
    ui->mdiArea->addSubWindow(table);
    table->show();
    table->displayTable();
    connect(this, &MainWindow::refractionExportFile, table, &MaterialRefractionTable::exportTable);
    connect(this, &MainWindow::refractionImportFile, table, &MaterialRefractionTable::importTable);
}

void MainWindow::exportRefraction()
{
    QString filename = QFileDialog::getSaveFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");
    emit refractionExportFile(filename);
}

void MainWindow::importRefraction()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");
    emit refractionImportFile(filename);
}

void MainWindow::makeCalc12()
{
    Calc12Window *calc = new Calc12Window();
    ui->mdiArea->addSubWindow(calc);
    calc->show();
}

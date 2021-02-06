#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spreadsheet.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Database *db;
    Spreadsheet *table;
    Spreadsheet *experiments;
    QString selectedExperiment;

    void setupToolbar();

private slots:
    void loadTable();
    void createParameterTable();
    void loadPlot();
    void runPython();
    void addNewExperiment();
    void openAllExperiments();
    void openExperimentSelector();
    void currentExperiment(QString uuid, QString name);
};

#endif // MAINWINDOW_H

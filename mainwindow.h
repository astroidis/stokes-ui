#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

signals:
    void refractionExportFile(QString filename);
    void refractionImportFile(QString filename);

private:
    Ui::MainWindow *ui;
    Database *db;
    QString defaultTitle;
    QString selectedExperimentUUID;

    void setupToolbar();

private slots:
    void createParameterTable();
    void loadPlot();
    void openAllExperiments();
    void currentExperiment(QString uuid, QString name);
    void openRefraction();
    void exportRefraction();
    void importRefraction();
    void makeCalc12();
    void makeCalc3();

};

#endif // MAINWINDOW_H

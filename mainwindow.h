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

private:
    Ui::MainWindow *ui;
    Database *db;
    QString defaultTitle;
    QString selectedExperimentUUID;

private slots:
    void openConstants();
    void loadPlot();
    void openAllExperiments();
    void setMenuEnabled();
    void currentExperiment(QString uuid, QString name);
    void openRefraction();
    void makeCalc12();
    void makeCalc3();

};

#endif // MAINWINDOW_H

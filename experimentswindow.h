#ifndef EXPERIMENTSWINDOW_H
#define EXPERIMENTSWINDOW_H

#include <QWidget>
#include <QSqlTableModel>
#include <QToolBar>

namespace Ui {
class ExperimentsWindow;
}

class ExperimentsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ExperimentsWindow(QWidget *parent = nullptr);
    ~ExperimentsWindow();


signals:
    void experimentSelected(QString uuid, QString name);

private:
    Ui::ExperimentsWindow *ui;
    QSqlTableModel *modelExp;
    QToolBar *tb;

    void setupToolbar();
    void setupTable();
    void importRaysData(QString expuuid, QString filename);
    void importC1Data(QString expuuid, QString filename);
    void importC2Data(QString expuuid, QString filename);
    void importC3Data(QString expuuid, QString filename);

private slots:
    void openAddDialog();
    void deleteExperiment();
    void selectExperiment();
    void importExperimentData(QString expuuid, QString frays, QString fc1, QString fc2, QString fc3);
//    void openLoadDialog();
//    void openUpdateDialog();
};

#endif // EXPERIMENTSWINDOW_H

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

private slots:
    void openAddDialog();
    void deleteExperiment();
    void selectExperiment();
//    void openLoadDialog();
//    void openUpdateDialog();
};

#endif // EXPERIMENTSWINDOW_H

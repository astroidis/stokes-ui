#ifndef EXPERIMENTSELECTORDIALOG_H
#define EXPERIMENTSELECTORDIALOG_H

#include <QDialog>
#include "database.h"

namespace Ui {
class ExperimentSelectorDialog;
}

class ExperimentSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExperimentSelectorDialog(QWidget *parent = nullptr);
    ~ExperimentSelectorDialog();

signals:
    void experimentSelected(QString uuid, QString name);

private:
    Ui::ExperimentSelectorDialog *ui;
    QSqlQuery experiments;

    void initCombo();

private slots:
    void loadComment();
    void makeChoice();
};

#endif // EXPERIMENTSELECTORDIALOG_H

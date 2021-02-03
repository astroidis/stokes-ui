#ifndef EDITEXPERIMENTDIALOG_H
#define EDITEXPERIMENTDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class EditExperimentDialog;
}

class EditExperimentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditExperimentDialog(QWidget *parent = nullptr);
    ~EditExperimentDialog();

    void initFieldData(QSqlRecord record);
    QString name() const;
    QString comment() const;

private:
    Ui::EditExperimentDialog *ui;
    QSqlRecord record;

private slots:
    void save();
};

#endif // EDITEXPERIMENTDIALOG_H

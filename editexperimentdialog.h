#ifndef EDITEXPERIMENTDIALOG_H
#define EDITEXPERIMENTDIALOG_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class EditExperimentDialog;
}

class EditExperimentDialog : public QDialog
{
    Q_OBJECT

public:
    QSqlRecord rec;
    bool is_accepted;

    explicit EditExperimentDialog(QWidget *parent = nullptr);
    ~EditExperimentDialog();

    void initFieldData(QSqlRecord record);

private:
    Ui::EditExperimentDialog *ui;

private slots:
    void save();
};

#endif // EDITEXPERIMENTDIALOG_H

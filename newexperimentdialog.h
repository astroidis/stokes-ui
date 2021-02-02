#ifndef NEWEXPERIMENTDIALOG_H
#define NEWEXPERIMENTDIALOG_H

#include <QDialog>

namespace Ui {
class NewExperimentDialog;
}

class NewExperimentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewExperimentDialog(QWidget *parent = nullptr);
    ~NewExperimentDialog();

private:
    Ui::NewExperimentDialog *ui;

private slots:
    void save();
};

#endif // NEWEXPERIMENTDIALOG_H

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

signals:
    void importFiles(QString expuuid, QString frays, QString fc1, QString fc2, QString fc3);
    void filename(QString name);

private:
    Ui::NewExperimentDialog *ui;
    QString filepath;
    QString new_name, new_comment;

private slots:
    void save();
    void selectFileRays();
    void selectFileC1();
    void selectFileC2();
    void selectFileC3();
};

#endif // NEWEXPERIMENTDIALOG_H

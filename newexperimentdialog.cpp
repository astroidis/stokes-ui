#include "newexperimentdialog.h"
#include "ui_newexperimentdialog.h"
#include "database.h"

NewExperimentDialog::NewExperimentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewExperimentDialog)
{
    ui->setupUi(this);
    connect(ui->buttonSave, &QPushButton::clicked, this, &NewExperimentDialog::save);
}

NewExperimentDialog::~NewExperimentDialog()
{
    delete ui;
}

void NewExperimentDialog::save()
{
    QString name = ui->editName->text();
    if (name.length() > 255){
        name.truncate(255);
    }

    QString comment = ui->editComment->toPlainText();
    if (comment.length() > 1024){
        comment.truncate(1024);
    }

    Database::instance()->addNewExperiment(name, comment);
}

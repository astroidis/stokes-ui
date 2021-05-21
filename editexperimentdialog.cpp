#include "editexperimentdialog.h"
#include "ui_editexperimentdialog.h"

#include <QDebug>

EditExperimentDialog::EditExperimentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditExperimentDialog)
{
    ui->setupUi(this);
    this->setFixedWidth(300);
    this->setModal(true);
    this->is_accepted = false;

    connect(ui->buttonSave, &QPushButton::clicked, this, &EditExperimentDialog::save);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &EditExperimentDialog::close);
}

EditExperimentDialog::~EditExperimentDialog()
{
    delete ui;
}

void EditExperimentDialog::initFieldData(QSqlRecord record)
{
    ui->editName->setText(record.value("obj_name").toString());
    ui->editComment->setPlainText(record.value("obj_comment").toString());
    ui->editCreated->setText(record.value("date_created").toString());
    ui->editCalculated->setText(record.value("date_calculated").toString());
    rec = record;
}

void EditExperimentDialog::save()
{
    rec.setValue("obj_name", ui->editName->text());
    rec.setValue("obj_comment", ui->editComment->toPlainText());
    is_accepted = true;

    accept();
}

#include "editexperimentdialog.h"
#include "ui_editexperimentdialog.h"
#include "database.h"
#include <QDebug>

EditExperimentDialog::EditExperimentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditExperimentDialog)
{
    ui->setupUi(this);

    connect(ui->buttonSave, &QPushButton::clicked, this, &EditExperimentDialog::save);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &EditExperimentDialog::close);
}

EditExperimentDialog::~EditExperimentDialog()
{
    delete ui;
}

void EditExperimentDialog::initFieldData(QSqlRecord record)
{
    ui->editId->setText(record.field("obj_id").value().toString());
    ui->editUuid->setText(record.field("obj_uuid").value().toString());
    ui->editName->setText(record.field("obj_name").value().toString());
    ui->editComment->setPlainText(record.field("obj_comment").value().toString());
    ui->editCreated->setText(record.field("date_created").value().toString());
    ui->editCalculated->setText(record.field("date_calculated").value().toString());
}

void EditExperimentDialog::save()
{
    bool r = Database::instance()->updateExperiment(ui->editId->text(),
                                           ui->editName->text(),
                                           ui->editComment->toPlainText());
    qDebug() << r;
    close();
}

QString EditExperimentDialog::name() const
{
    return ui->editName->text();
}

QString EditExperimentDialog::comment() const
{
    return ui->editComment->toPlainText();
}

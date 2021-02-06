#include "experimentselectordialog.h"
#include "ui_experimentselectordialog.h"

#include "database.h"
#include <QDebug>

ExperimentSelectorDialog::ExperimentSelectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperimentSelectorDialog)
{
    ui->setupUi(this);
    initCombo();
    connect(ui->comboName, &QComboBox::textActivated, this, &ExperimentSelectorDialog::loadComment);
    connect(ui->buttonSelect, &QPushButton::clicked, this, &ExperimentSelectorDialog::makeChoice);
}

ExperimentSelectorDialog::~ExperimentSelectorDialog()
{
    delete ui;
}

void ExperimentSelectorDialog::initCombo()
{
    experiments = Database::instance()->getAllExperiments();
    experiments.exec();
    int colno = experiments.record().indexOf("obj_name");
    QStringList names;
    names << "<Select>";
    while (experiments.next())
        names << experiments.value(colno).toString();

    ui->comboName->addItems(names);
}

void ExperimentSelectorDialog::loadComment()
{
    int index = ui->comboName->currentIndex();
    experiments.seek(index - 1);
    index = experiments.record().indexOf("obj_comment");
    ui->editComment->setPlainText(experiments.value(index).toString());
}

void ExperimentSelectorDialog::makeChoice()
{
    int namecol = experiments.record().indexOf("obj_name");
    int uuidcol = experiments.record().indexOf("obj_uuid");
    experiments.seek(ui->comboName->currentIndex() - 1);
    emit experimentSelected(experiments.value(uuidcol).toString(),
                            experiments.value(namecol).toString());
    this->close();
}

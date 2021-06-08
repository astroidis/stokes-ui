#include "newexperimentdialog.h"
#include "ui_newexperimentdialog.h"

#include <QFileDialog>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QSqlError>

NewExperimentDialog::NewExperimentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewExperimentDialog)
{
    ui->setupUi(this);
    connect(ui->buttonSave, &QPushButton::clicked, this, &NewExperimentDialog::save);
    connect(ui->buttonClose, &QPushButton::clicked, this, &NewExperimentDialog::close);
    connect(ui->buttonRaysFile, &QPushButton::clicked, this, &NewExperimentDialog::selectFileRays);
    connect(ui->buttonC1File, &QPushButton::clicked, this, &NewExperimentDialog::selectFileC1);
    connect(ui->buttonC2File, &QPushButton::clicked, this, &NewExperimentDialog::selectFileC2);
    connect(ui->buttonC3File, &QPushButton::clicked, this, &NewExperimentDialog::selectFileC3);
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

    new_name = name;
    new_comment = comment;

    QSqlQuery query = QSqlQuery(QSqlDatabase::database("stokes_db"));
    query.prepare("insert into experiments (obj_name, obj_comment)"
                  "values (?, ?)"
                  "returning obj_uuid");
    query.addBindValue(new_name);
    query.addBindValue(new_comment);
    query.exec();

    QString expuuid;
    while (query.next()){
        expuuid = query.value(0).toString();
    }

    emit importFiles(expuuid, ui->editRaysFile->text(), ui->editC1File->text(),
                    ui->editC2File->text(), ui->editC3File->text());
    accept();
}

void NewExperimentDialog::selectFileC1()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editC1File->setText(filename);
}

void NewExperimentDialog::selectFileC2()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editC2File->setText(filename);
}

void NewExperimentDialog::selectFileC3()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editC3File->setText(filename);
}

void NewExperimentDialog::selectFileRays()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editRaysFile->setText(filename);
}

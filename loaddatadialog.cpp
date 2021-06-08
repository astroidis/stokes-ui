#include "loaddatadialog.h"
#include "ui_loaddatadialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

LoadDataDialog::LoadDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadDataDialog)
{
    ui->setupUi(this);

    connect(ui->buttonCancel, &QPushButton::clicked, this, &LoadDataDialog::close);
    connect(ui->buttonC1, &QPushButton::clicked, this, &LoadDataDialog::selectFileC1);
    connect(ui->buttonC2, &QPushButton::clicked, this, &LoadDataDialog::selectFileC2);
    connect(ui->buttonC3, &QPushButton::clicked, this, &LoadDataDialog::selectFileC3);
    connect(ui->buttonRays, &QPushButton::clicked, this, &LoadDataDialog::selectFileRays);
}

LoadDataDialog::~LoadDataDialog()
{
    delete ui;
}

void LoadDataDialog::deleteOldData()
{

}

void LoadDataDialog::loadNewData()
{
    int ret = QMessageBox::warning(this, "Загрузка", "Старые данные будут удалены\nПродолжить?",
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok){

    }
}

void LoadDataDialog::selectFileRays()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editRays->setText(filename);
}

void LoadDataDialog::selectFileC1()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editC1->setText(filename);
}

void LoadDataDialog::selectFileC2()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editC2->setText(filename);
}

void LoadDataDialog::selectFileC3()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "C:\\Users\\User\\Documents\\",
                                                    "CSV (*.csv) ;; Text (*.txt) ;; All files (*.*)");

    ui->editC3->setText(filename);
}

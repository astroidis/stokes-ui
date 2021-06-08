#include "logfilewidget.h"
#include "ui_logfilewidget.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

LogFileWidget::LogFileWidget(QString file, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogFileWidget),
    filename(file)
{
    ui->setupUi(this);

    openFile();
}

LogFileWidget::~LogFileWidget()
{
    delete ui;
}

void LogFileWidget::openFile()
{
    QFile file(filename);
    ui->plainTextEdit->clear();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file.errorString();
    }

    QTextStream in(&file);
    ui->plainTextEdit->setPlainText(in.readAll());

    file.close();
}

#include "calc12window.h"
#include "ui_calc12window.h"

Calc12Window::Calc12Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc12Window)
{
    ui->setupUi(this);
}

Calc12Window::~Calc12Window()
{
    delete ui;
}

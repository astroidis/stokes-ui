#include "calc3window.h"
#include "ui_calc3window.h"

Calc3Window::Calc3Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc3Window)
{
    ui->setupUi(this);
}

Calc3Window::~Calc3Window()
{
    delete ui;
}

#include "calc12window.h"
#include "ui_calc12window.h"

#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

#include "calculations/task12.h"


Calc12Window::Calc12Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc12Window),
    tb(new QToolBar(this))
{
    ui->setupUi(this);
    openTable();
    setupToolbar();

    connect(ui->rCalcAllButton, &QPushButton::clicked, this, &Calc12Window::makeCalculations);
    connect(ui->rCalcButton, &QPushButton::clicked, this, &Calc12Window::calculateOne);
}

Calc12Window::~Calc12Window()
{
    delete ui;
}

void Calc12Window::openTable()
{
    model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("calculation_12");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->hideColumn(0);
}

void Calc12Window::makeCalculations()
{
    for (int i = 0; i < model->rowCount(); i++){
        QSqlRecord rec = model->record(i);
        double alfa = rec.value("Alfa").toDouble();
        double beta = rec.value("Beta").toDouble();
        Task12 t(i, alfa, beta);
        t.loadIntensities(0,
                          rec.value("I1").toDouble(),
                          rec.value("Tau1").toDouble(),
                          rec.value("Phi1").toDouble());
        t.loadIntensities(1,
                          rec.value("I2").toDouble(),
                          rec.value("Tau2").toDouble(),
                          rec.value("Phi2").toDouble());
        t.loadIntensities(2,
                          rec.value("I3").toDouble(),
                          rec.value("Tau3").toDouble(),
                          rec.value("Phi3").toDouble());
        t.loadIntensities(3,
                          rec.value("I4").toDouble(),
                          rec.value("Tau4").toDouble(),
                          rec.value("Phi4").toDouble());

        auto vectors = t.calcRadiation(std::complex(1.4, -4.53), 52.0);
        auto sv = vectors.first;
        auto nsv = vectors.second;
        auto gr = Calculation::Gradient();
        bool a = true;
        if (ui->analyticRBtn->isChecked()){
            a = true;
        }
        else if (ui->numericRBtn->isChecked()){
            a = false;
        }
        Reflection ref = t.calcReflection(std::complex(1.4, -4.53), 52.0, gr, a);

        rec.setValue("J", QString::number(sv.J, 'f', 4));
        rec.setValue("Q", QString::number(sv.Q, 'f', 4));
        rec.setValue("U", QString::number(sv.U, 'f', 4));
        rec.setValue("V", QString::number(sv.V, 'f', 4));
        rec.setValue("P", QString::number(sv.p(), 'f', 4));
        rec.setValue("J0", QString::number(nsv.J, 'f', 4));
        rec.setValue("Q0", QString::number(nsv.Q, 'f', 4));
        rec.setValue("U0", QString::number(nsv.U, 'f', 4));
        rec.setValue("V0", QString::number(nsv.V, 'f', 4));
        rec.setValue("P0", QString::number(nsv.p(), 'f', 4));
        rec.setValue("Alfa1", QString::number(ref.alfa1, 'f', 4));
        rec.setValue("Beta1", QString::number(ref.beta1, 'f', 4));
        rec.setValue("Re_Hi", QString::number(ref.re_hi, 'f', 4));
        rec.setValue("Im_Hi", QString::number(ref.im_hi, 'f', 4));
        model->setRecord(i, rec);
    }
}

void Calc12Window::saveData()
{
    model->submitAll();
}

void Calc12Window::loadData()
{
    QMessageBox msg(QMessageBox::Warning, "Warning", "Unsaved data will be lost");
    msg.addButton("Continue", QMessageBox::AcceptRole);
    msg.addButton("Cancel", QMessageBox::RejectRole);
    switch (msg.exec()){
        case QMessageBox::AcceptRole:
            qDebug() << "Reloading data\n";
            model->select();
            ui->tableView->reset();
            break;

        case QMessageBox::RejectRole:
            qDebug() << "Loading canceled\n";
            msg.close();
            break;

        default:
            qDebug() << "Closing dialog\n";
            msg.close();
            break;
    }

}

void Calc12Window::calculateOne()
{
    QModelIndex ind = ui->tableView->selectionModel()->selectedRows().back();
    int i = ind.row();
    QSqlRecord rec = model->record(i);
    double alfa = rec.value("Alfa").toDouble();
    double beta = rec.value("Beta").toDouble();
    Task12 t(i, alfa, beta);
    t.loadIntensities(0,
                      rec.value("I1").toDouble(),
                      rec.value("Tau1").toDouble(),
                      rec.value("Phi1").toDouble());
    t.loadIntensities(1,
                      rec.value("I2").toDouble(),
                      rec.value("Tau2").toDouble(),
                      rec.value("Phi2").toDouble());
    t.loadIntensities(2,
                      rec.value("I3").toDouble(),
                      rec.value("Tau3").toDouble(),
                      rec.value("Phi3").toDouble());
    t.loadIntensities(3,
                      rec.value("I4").toDouble(),
                      rec.value("Tau4").toDouble(),
                      rec.value("Phi4").toDouble());

    auto vectors = t.calcRadiation(std::complex(1.4, -4.53), 52.0);
    auto sv = vectors.first;
    auto nsv = vectors.second;
    auto gr = Calculation::Gradient();
    bool a = true;
    if (ui->analyticRBtn->isChecked()){
        a = true;
    }
    else if (ui->numericRBtn->isChecked()){
        a = false;
    }
    Reflection ref = t.calcReflection(std::complex(1.4, -4.53), 52.0, gr, a);

    rec.setValue("J", QString::number(sv.J, 'f', 4));
    rec.setValue("Q", QString::number(sv.Q, 'f', 4));
    rec.setValue("U", QString::number(sv.U, 'f', 4));
    rec.setValue("V", QString::number(sv.V, 'f', 4));
    rec.setValue("P", QString::number(sv.p(), 'f', 4));
    rec.setValue("J0", QString::number(nsv.J, 'f', 4));
    rec.setValue("Q0", QString::number(nsv.Q, 'f', 4));
    rec.setValue("U0", QString::number(nsv.U, 'f', 4));
    rec.setValue("V0", QString::number(nsv.V, 'f', 4));
    rec.setValue("P0", QString::number(nsv.p(), 'f', 4));
    rec.setValue("Alfa1", QString::number(ref.alfa1, 'f', 4));
    rec.setValue("Beta1", QString::number(ref.beta1, 'f', 4));
    rec.setValue("Re_Hi", QString::number(ref.re_hi, 'f', 4));
    rec.setValue("Im_Hi", QString::number(ref.im_hi, 'f', 4));
    model->setRecord(i, rec);
}

void Calc12Window::setupToolbar()
{
//    tb->setOrientation(Qt::Horizontal);

    QAction *saveAct = new QAction("Save", this);
    tb->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &Calc12Window::saveData);

    QAction *loadAct = new QAction("Load", this);
    tb->addAction(loadAct);
    connect(loadAct, &QAction::triggered, this, &Calc12Window::loadData);

    ui->toolLayout->addWidget(tb);
    tb->show();
}

#include "calc12window.h"
#include "ui_calc12window.h"

#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

#include "calculations/task12.h"
#include "logger.h"


Calc12Window::Calc12Window(QString experiment_id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc12Window),
    tb(new QToolBar(this)),
    experiment(experiment_id)
{
    ui->setupUi(this);
    openTable();
    setupToolbar();

    connect(ui->rCalcAllButton, &QPushButton::clicked, this, &Calc12Window::makeCalculations);
    connect(ui->rCalcButton, &QPushButton::clicked, this, &Calc12Window::calculateOne);
    connect(ui->anglesCheck, &QCheckBox::clicked, this, &Calc12Window::displayTauPhi);
    connect(ui->dispRaysCheck, &QCheckBox::clicked, this, &Calc12Window::displayDispRays);
    connect(ui->intensityCheck, &QCheckBox::clicked, this, &Calc12Window::displayIntensity);
    connect(ui->natRaysCheck, &QCheckBox::clicked, this, &Calc12Window::displayNatRays);
    connect(ui->polarCheck, &QCheckBox::clicked, this, &Calc12Window::displayPolar);
    connect(ui->dropCalcButton, &QPushButton::clicked, this, &Calc12Window::dropCalculation);
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
    model->setFilter(QString("experiment_id = '%1'").arg(experiment));
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(model->fieldIndex("experiment_id"));
}

void Calc12Window::makeCalculations()
{
    Logger logger("rays.log");
    logger.logInfo("Вычисление параметров излучения");
    for (int j = 0; j < model->rowCount(); j++){
        QSqlRecord rec = model->record(j);
        double alfa = rec.value("Alfa").toDouble();
        double beta = rec.value("Beta").toDouble();
        Task12 t(j, alfa, beta);

        logger.logInfo(QString("Task12(%1, %2, %3)").arg(j).arg(alfa, 0, 'g', 3).arg(beta, 0, 'g', 3));

        double i = rec.value("I1").toDouble();
        double phi = rec.value("Tau1").toDouble();
        double tau = rec.value("Phi1").toDouble();
        logger.logInfo(QString("Intensity(0, %1, %2, %3)").arg(i, 0, 'g', 3)
                       .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
        t.loadIntensities(0, i, phi, tau);

        i = rec.value("I2").toDouble();
        phi = rec.value("Tau2").toDouble();
        tau = rec.value("Phi2").toDouble();
        logger.logInfo(QString("Intensity(1, %1, %2, %3)").arg(i, 0, 'g', 3)
                       .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
        t.loadIntensities(1, i, phi, tau);

        i = rec.value("I3").toDouble();
        phi = rec.value("Tau3").toDouble();
        tau = rec.value("Phi3").toDouble();
        logger.logInfo(QString("Intensity(2, %1, %2, %3)").arg(i, 0, 'g', 3)
                       .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
        t.loadIntensities(2, i, phi, tau);

        i = rec.value("I4").toDouble();
        tau = rec.value("Tau4").toDouble();
        phi = rec.value("Phi4").toDouble();
        logger.logInfo(QString("Intensity(3, %1, %2, %3)").arg(i, 0, 'g', 3)
                       .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
        t.loadIntensities(3, i, tau, phi);

        auto vectors = t.calcRadiation(std::complex(1.4, -4.53), 52.0);
        auto sv = vectors.first;
        auto nsv = vectors.second;
        auto gr = Calculation::Gradient();
        bool a = ui->analyticRBtn->isChecked();

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
    QMessageBox msg(QMessageBox::Warning, "Warning", "Несохраненные даныее будут удалены");
    msg.addButton("Продолжить", QMessageBox::AcceptRole);
    msg.addButton("Отмена", QMessageBox::RejectRole);
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
    if (ui->tableView->selectionModel()->selectedRows().isEmpty()){
        return;
    }

    Logger logger("rays.log");
    logger.logInfo("Вычисление параметров излучения");

    QModelIndex ind = ui->tableView->selectionModel()->selectedRows().back();
    int index = ind.row();
    QSqlRecord rec = model->record(index);
    double alfa = rec.value("Alfa").toDouble();
    double beta = rec.value("Beta").toDouble();

    logger.logInfo(QString("Task12(%1, %2, %3)").arg(index).arg(alfa, 0, 'g', 3).arg(beta, 0, 'g', 3));

    Task12 t(index, alfa, beta);
    double i = rec.value("I1").toDouble();
    double phi = rec.value("Tau1").toDouble();
    double tau = rec.value("Phi1").toDouble();
    logger.logInfo(QString("Intensity(0, %1, %2, %3)").arg(i, 0, 'g', 3)
                   .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
    t.loadIntensities(0, i, phi, tau);

    i = rec.value("I2").toDouble();
    phi = rec.value("Tau2").toDouble();
    tau = rec.value("Phi2").toDouble();
    logger.logInfo(QString("Intensity(1, %1, %2, %3)").arg(i, 0, 'g', 3)
                   .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
    t.loadIntensities(1, i, phi, tau);

    i = rec.value("I3").toDouble();
    phi = rec.value("Tau3").toDouble();
    tau = rec.value("Phi3").toDouble();
    logger.logInfo(QString("Intensity(2, %1, %2, %3)").arg(i, 0, 'g', 3)
                   .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
    t.loadIntensities(2, i, phi, tau);

    i = rec.value("I4").toDouble();
    tau = rec.value("Tau4").toDouble();
    phi = rec.value("Phi4").toDouble();
    logger.logInfo(QString("Intensity(3, %1, %2, %3)").arg(i, 0, 'g', 3)
                   .arg(phi, 0, 'g', 3).arg(tau, 0, 'g', 3));
    t.loadIntensities(3, i, tau, phi);

    auto vectors = t.calcRadiation(std::complex(1.4, -4.53), 52.0);
    auto sv = vectors.first;
    auto nsv = vectors.second;
    auto gr = Calculation::Gradient();
    bool a = ui->analyticRBtn->isChecked();

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
    model->setRecord(index, rec);
}

void Calc12Window::displayTauPhi()
{
    if (ui->anglesCheck->isChecked()){
        ui->tableView->showColumn(model->fieldIndex("Tau1"));
        ui->tableView->showColumn(model->fieldIndex("Tau2"));
        ui->tableView->showColumn(model->fieldIndex("Tau3"));
        ui->tableView->showColumn(model->fieldIndex("Tau4"));
        ui->tableView->showColumn(model->fieldIndex("Phi1"));
        ui->tableView->showColumn(model->fieldIndex("Phi2"));
        ui->tableView->showColumn(model->fieldIndex("Phi3"));
        ui->tableView->showColumn(model->fieldIndex("Phi4"));
    }
    else{
        ui->tableView->hideColumn(model->fieldIndex("Tau1"));
        ui->tableView->hideColumn(model->fieldIndex("Tau2"));
        ui->tableView->hideColumn(model->fieldIndex("Tau3"));
        ui->tableView->hideColumn(model->fieldIndex("Tau4"));
        ui->tableView->hideColumn(model->fieldIndex("Phi1"));
        ui->tableView->hideColumn(model->fieldIndex("Phi2"));
        ui->tableView->hideColumn(model->fieldIndex("Phi3"));
        ui->tableView->hideColumn(model->fieldIndex("Phi4"));
    }
}

void Calc12Window::displayIntensity()
{
    if (ui->intensityCheck->isChecked()){
        ui->tableView->showColumn(model->fieldIndex("I1"));
        ui->tableView->showColumn(model->fieldIndex("I2"));
        ui->tableView->showColumn(model->fieldIndex("I3"));
        ui->tableView->showColumn(model->fieldIndex("I4"));
    }
    else{
        ui->tableView->hideColumn(model->fieldIndex("I1"));
        ui->tableView->hideColumn(model->fieldIndex("I2"));
        ui->tableView->hideColumn(model->fieldIndex("I3"));
        ui->tableView->hideColumn(model->fieldIndex("I4"));
    }
}

void Calc12Window::displayNatRays()
{
    if (ui->natRaysCheck->isChecked()){
        ui->tableView->showColumn(model->fieldIndex("J0"));
        ui->tableView->showColumn(model->fieldIndex("Q0"));
        ui->tableView->showColumn(model->fieldIndex("U0"));
        ui->tableView->showColumn(model->fieldIndex("V0"));
        ui->tableView->showColumn(model->fieldIndex("P0"));
    }
    else{
        ui->tableView->hideColumn(model->fieldIndex("J0"));
        ui->tableView->hideColumn(model->fieldIndex("Q0"));
        ui->tableView->hideColumn(model->fieldIndex("U0"));
        ui->tableView->hideColumn(model->fieldIndex("V0"));
        ui->tableView->hideColumn(model->fieldIndex("P0"));
    }
}

void Calc12Window::displayDispRays()
{
    if (ui->dispRaysCheck->isChecked()){
        ui->tableView->showColumn(model->fieldIndex("J"));
        ui->tableView->showColumn(model->fieldIndex("Q"));
        ui->tableView->showColumn(model->fieldIndex("U"));
        ui->tableView->showColumn(model->fieldIndex("V"));
        ui->tableView->showColumn(model->fieldIndex("P"));
    }
    else{
        ui->tableView->hideColumn(model->fieldIndex("J"));
        ui->tableView->hideColumn(model->fieldIndex("Q"));
        ui->tableView->hideColumn(model->fieldIndex("U"));
        ui->tableView->hideColumn(model->fieldIndex("V"));
        ui->tableView->hideColumn(model->fieldIndex("P"));
    }
}

void Calc12Window::displayPolar()
{
    if (ui->polarCheck->isChecked()){
        ui->tableView->showColumn(model->fieldIndex("Alfa1"));
        ui->tableView->showColumn(model->fieldIndex("Beta1"));
        ui->tableView->showColumn(model->fieldIndex("Re_Hi"));
        ui->tableView->showColumn(model->fieldIndex("Im_Hi"));
    }
    else {
        ui->tableView->hideColumn(model->fieldIndex("Alfa1"));
        ui->tableView->hideColumn(model->fieldIndex("Beta1"));
        ui->tableView->hideColumn(model->fieldIndex("Re_Hi"));
        ui->tableView->hideColumn(model->fieldIndex("Im_Hi"));
    }
}

void Calc12Window::dropCalculation()
{
    for (int i = 0; i < model->rowCount(); i++){
        QSqlRecord rec = model->record(i);
        rec.setValue("J", 0);
        rec.setValue("Q", 0);
        rec.setValue("U", 0);
        rec.setValue("V", 0);
        rec.setValue("P", 0);
        rec.setValue("J0", 0);
        rec.setValue("Q0", 0);
        rec.setValue("U0", 0);
        rec.setValue("V0", 0);
        rec.setValue("P0", 0);
        rec.setValue("Alfa1", 0);
        rec.setValue("Beta1", 0);
        rec.setValue("Re_Hi", 0);
        rec.setValue("Im_Hi", 0);
        model->setRecord(i, rec);
    }
}

void Calc12Window::setupToolbar()
{
    QAction *saveAct = new QAction("Сохранить", this);
    tb->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &Calc12Window::saveData);

    QAction *loadAct = new QAction("Загрузить", this);
    tb->addAction(loadAct);
    connect(loadAct, &QAction::triggered, this, &Calc12Window::loadData);

    ui->toolLayout->addWidget(tb);
    tb->show();
}

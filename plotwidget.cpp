#include "plotwidget.h"
#include "ui_plotwidget.h"

#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

using namespace QtDataVisualization;

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
{
    ui->setupUi(this);
    setupButtonGroup();
    plot("J");
}

PlotWidget::~PlotWidget()
{
    delete ui;
}

void PlotWidget::plot(QString argument)
{
    QSurfaceDataArray *data = new QSurfaceDataArray();

    QSqlQueryModel model = QSqlQueryModel();
    model.setQuery("select distinct \"Alfa\" from calculation_12 order by \"Alfa\" asc",
                   QSqlDatabase::database("stokes_db"));

    for (int i = 0; i < model.rowCount(); i++){
        QSurfaceDataRow *row = new QSurfaceDataRow();

        QSqlRecord rec = model.record(i);
        double alfa = rec.value("Alfa").toDouble();

        QSqlQuery q = QSqlQuery(QSqlDatabase::database("stokes_db"));
        q.prepare(QString("select \"Beta\", \"%1\" from calculation_12 where \"Alfa\" = :val order by \"Beta\" asc;").arg(argument));
        q.bindValue(":val", alfa);
        q.exec();

        while(q.next()){
            double beta = q.value("Beta").toDouble();
            double arg = q.value(argument).toDouble();
            QSurfaceDataItem item1 = QSurfaceDataItem();
            item1.setX(beta);
            item1.setY(arg);
            item1.setZ(alfa);

            row->append(item1);
        }

        data->append(row);
    }

    QSurface3DSeries *series = new QSurface3DSeries();
    series->dataProxy()->resetArray(data);

    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    series->setBaseGradient(gr);
    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    series->setFlatShadingEnabled(false);

    Q3DSurface *surface = new Q3DSurface();
    surface->addSeries(series);

    surface->axisX()->setTitle("Beta");
    surface->axisX()->setTitleVisible(true);

    surface->axisY()->setTitle(argument);
    surface->axisY()->setTitleVisible(true);

    surface->axisZ()->setTitle("Alfa");
    surface->axisZ()->setTitleVisible(true);
    surface->axisZ()->setReversed(true);

    surface->setShadowQuality(Q3DSurface::ShadowQualityNone);

    container = QWidget::createWindowContainer(surface);
    ui->horizontalLayout->addWidget(container);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->show();
}

void PlotWidget::setupButtonGroup()
{
    group = new QButtonGroup();
    group->addButton(ui->rbJ);
    group->addButton(ui->rbJ0);
    group->addButton(ui->rbP);
    group->addButton(ui->rbP0);
    group->addButton(ui->rbQ);
    group->addButton(ui->rbQ0);
    group->addButton(ui->rbU);
    group->addButton(ui->rbU0);
    group->addButton(ui->rbV);
    group->addButton(ui->rbV0);

    connect(group, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(replot(QAbstractButton *)));
}

void PlotWidget::replot(QAbstractButton *button)
{
    delete container;

    QString parameter = button->text();
    plot(parameter);
}

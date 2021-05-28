#include "plotwidget.h"

#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>

using namespace QtDataVisualization;


PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent)
    , surface(new Q3DSurface())
{
}

Q3DSurface *PlotWidget::plot(QString argument)
{
    QSqlQueryModel model = QSqlQueryModel();
    model.setQuery("select \"Alfa\", \"Beta\", \"J\" from calculation_12 order by \"J\" asc, \"Alfa\" asc",
                   QSqlDatabase::database("stokes_db"));
    qDebug() << model.lastError();

    QSurfaceDataArray *data = new QSurfaceDataArray();
//    QSurfaceDataRow *dataRow1 = new QSurfaceDataRow();
//    QSurfaceDataRow *dataRow2 = new QSurfaceDataRow();
//    *dataRow1 << QVector3D(0, 0, 0) << QVector3D(1, 0, 0);
//    *dataRow2 << QVector3D(0, 0, 1) << QVector3D(0, 1, 1);
//    *data << dataRow1 << dataRow2;
    for (int i = 0; i < model.rowCount()-3; i+=3){
        QSurfaceDataRow *row = new QSurfaceDataRow();

        QSqlRecord rec = model.record(i);
        double arg = rec.value(argument).toDouble();
        double alfa = rec.value("Alfa").toDouble();
        double beta = rec.value("Beta").toDouble();
        QSurfaceDataItem item1 = QSurfaceDataItem(QVector3D(alfa, beta, arg));

        rec = model.record(i+1);
        arg = rec.value(argument).toDouble();
        alfa = rec.value("Alfa").toDouble();
        beta = rec.value("Beta").toDouble();
        QSurfaceDataItem item2 = QSurfaceDataItem(QVector3D(alfa, beta, arg));

        rec = model.record(i+3);
        arg = rec.value(argument).toDouble();
        alfa = rec.value("Alfa").toDouble();
        beta = rec.value("Beta").toDouble();
        QSurfaceDataItem item3 = QSurfaceDataItem(QVector3D(alfa, beta, arg));

        row->append(item1);
        row->append(item2);
        row->append(item3);
        data->append(row);
    }
//    data->append(row);
//    }

    QSurface3DSeries *series = new QSurface3DSeries();
    series->dataProxy()->resetArray(data);
    surface->addSeries(series);
//    surface->axisX()->setTitle("Alfa");
//    surface->axisY()->setTitle("Beta");
//    surface->axisZ()->setTitle(argument);
//    series->setDrawMode(QSurface3DSeries::DrawSurface);

    return surface;
}

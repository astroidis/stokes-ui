#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QtDataVisualization/Q3DSurface>

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
//    QtDataVisualization::Q3DSurface *surface;

    explicit PlotWidget(QWidget *parent = nullptr);

    QtDataVisualization::Q3DSurface *plot(QString argument);

private:
    QtDataVisualization::Q3DSurface *surface;

};

#endif // PLOTWIDGET_H

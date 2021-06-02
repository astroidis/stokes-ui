#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QtDataVisualization/Q3DSurface>

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWidget(QString experiment_id, QWidget *parent = nullptr);
    ~PlotWidget();

private:
    Ui::PlotWidget *ui;
    QWidget *container;
    QButtonGroup *group;
    QString experiment;

    void plot(QString argument);
    void setupButtonGroup();

private slots:
    void replot(QAbstractButton *button);
};

#endif // PLOTWIDGET_H

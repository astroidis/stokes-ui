#ifndef CALC12WINDOW_H
#define CALC12WINDOW_H

#include <QWidget>
#include <QSqlTableModel>
#include <QToolBar>

namespace Ui {
class Calc12Window;
}

class Calc12Window : public QWidget
{
    Q_OBJECT

public:
    explicit Calc12Window(QWidget *parent = nullptr);
    ~Calc12Window();

    void openTable();

private slots:
    void makeCalculations();
    void saveData();
    void loadData();
    void calculateOne();
    void displayTauPhi();
    void displayIntensity();
    void displayNatRays();
    void displayDispRays();
    void displayPolar();

private:
    Ui::Calc12Window *ui;
    QSqlTableModel *model;
    QToolBar *tb;

    void setupToolbar();
};

#endif // CALC12WINDOW_H

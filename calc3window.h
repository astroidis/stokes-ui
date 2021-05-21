#ifndef CALC3WINDOW_H
#define CALC3WINDOW_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QToolBar>

namespace Ui {
class Calc3Window;
}

class Calc3Window : public QWidget
{
    Q_OBJECT

public:
    explicit Calc3Window(QWidget *parent = nullptr);
    ~Calc3Window();

private:
    Ui::Calc3Window *ui;
    QSqlTableModel *modelC1Calc;
    QSqlTableModel *modelC2Calc;
    QSqlTableModel *modelC3Calc;
    QSqlTableModel *modelStats;
    QToolBar *tb;

    void calcC1();
    void calcC2();
    void calcC3();
    void setupToolbar();

private slots:
    void c1rbtnClicked();
    void c2rbtnClicked();
    void c3rbtnClicked();
    void calcrbtnClicked();
//    void srcrbtnClicked();
    void calcParamPbtnClicked();
    void calcStats();
    void loadData();
    void saveData();
};

#endif // CALC3WINDOW_H

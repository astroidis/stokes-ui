#ifndef CALC3WINDOW_H
#define CALC3WINDOW_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>

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
    QTableView *c1Calculated;
    QTableView *c2Calculated;
    QTableView *c3Calculated;
    QSqlTableModel *modelC1Calc;
    QSqlTableModel *modelC2Calc;
    QSqlTableModel *modelC3Calc;

private slots:
    void c1rbtnClicked();
    void c2rbtnClicked();
    void c3rbtnClicked();
    void calcrbtnClicked();
//    void srcrbtnClicked();
};

#endif // CALC3WINDOW_H

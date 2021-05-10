#ifndef CALC3WINDOW_H
#define CALC3WINDOW_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTreeWidget>

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
    QSqlQueryModel *modelC1Stat;
    QSqlQueryModel *modelC2Stat;
    QSqlQueryModel *modelC3Stat;
    QTableView *tableC1Stat;
    QTableView *tableC2Stat;
    QTableView *tableC3Stat;
    QTreeWidgetItem *c1Item;
    QTreeWidgetItem *c2Item;
    QTreeWidgetItem *c3Item;
    QTreeWidgetItem *c1Child;
    QTreeWidgetItem *c2Child;
    QTreeWidgetItem *c3Child;

    void calcC1();
    void calcC2();
    void calcC3();

private slots:
    void c1rbtnClicked();
    void c2rbtnClicked();
    void c3rbtnClicked();
    void calcrbtnClicked();
//    void srcrbtnClicked();
    void calcParamPbtnClicked();
};

#endif // CALC3WINDOW_H

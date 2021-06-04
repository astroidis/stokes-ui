#ifndef MATERIALREFRACTIONTABLE_H
#define MATERIALREFRACTIONTABLE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>

namespace Ui {
class MaterialRefractionTable;
}

class MaterialRefractionTable : public QWidget
{
    Q_OBJECT

public:
    explicit MaterialRefractionTable(QWidget *parent = nullptr);
    ~MaterialRefractionTable();

    void displayTable();

private:
    Ui::MaterialRefractionTable *ui;
    QSqlTableModel *model;
};

#endif // MATERIALREFRACTIONTABLE_H

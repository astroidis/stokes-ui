#include "spreadsheet.h"
#include <QHeaderView>

Spreadsheet::Spreadsheet(QWidget *parent) : QWidget(parent)
{
}

void Spreadsheet::displayTable(const char *name)
{
    model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setTable(name);
    model->select();

    tableview = new QTableView(this);
    tableview->setModel(model);
    tableview->resizeColumnsToContents();
    tableview->resizeRowsToContents();
}

#include "spreadsheet.h"
#include <QScrollBar>
#include <QHeaderView>

Spreadsheet::Spreadsheet(QWidget *parent) : QWidget(parent)
  , layout (new QHBoxLayout)
{
    this->setLayout(layout);
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

    this->layout->addWidget(tableview);
}

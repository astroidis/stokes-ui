#include "spreadsheet.h"
#include "roweditingdelegate.h"
#include <QScrollBar>

Spreadsheet::Spreadsheet(QWidget *parent) : QWidget(parent)
  , layout (new QHBoxLayout)
{
    this->setLayout(layout);
//    connect(tableview, &QTableView::doubleClicked,
//            this, &Spreadsheet::openEditor);
}

void Spreadsheet::displayTable(const char *name)
{
    model = new QSqlTableModel(this, QSqlDatabase::database("stokes_db"));
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable(name);
    model->select();

    tableview = new QTableView(this);
    tableview->setModel(model);
    tableview->resizeColumnsToContents();
    tableview->resizeRowsToContents();
    tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableview->setItemDelegate(new RowEditingDelegate);

    this->layout->addWidget(tableview);
}

void Spreadsheet::openEditor(int row)
{
//    QSqlRecord rec = model->record(row);
//    EditExperimentDialog dlg(rec, this);
//    dlg.exec();
}

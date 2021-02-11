#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include <QHBoxLayout>

class Spreadsheet : public QWidget
{
    Q_OBJECT
public:
    explicit Spreadsheet(QWidget *parent = nullptr);

    void displayTable(const char *name);

private:
    QSqlTableModel *model;
    QTableView *tableview;
    QHBoxLayout *layout;
};

#endif // SPREADSHEET_H

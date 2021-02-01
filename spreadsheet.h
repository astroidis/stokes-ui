#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QWidget>
#include <QtSql>
#include <QTableView>

class Spreadsheet : public QWidget
{
    Q_OBJECT
public:
    explicit Spreadsheet(QWidget *parent = nullptr);

    void displayTable(const char *name);

signals:

private:
    QSqlTableModel *model;
    QTableView *tableview;
};

#endif // SPREADSHEET_H

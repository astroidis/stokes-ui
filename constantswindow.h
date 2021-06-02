#ifndef CONSTANTSWINDOW_H
#define CONSTANTSWINDOW_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class ConstantsWindow;
}

class ConstantsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConstantsWindow(QWidget *parent = nullptr);
    ~ConstantsWindow();

private:
    Ui::ConstantsWindow *ui;
    QSqlTableModel *model;

    void setupTable();
};

#endif // CONSTANTSWINDOW_H

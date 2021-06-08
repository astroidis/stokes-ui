#ifndef LOADDATADIALOG_H
#define LOADDATADIALOG_H

#include <QDialog>

namespace Ui {
class LoadDataDialog;
}

class LoadDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadDataDialog(QWidget *parent = nullptr);
    ~LoadDataDialog();

private:
    Ui::LoadDataDialog *ui;

    void deleteOldData();

private slots:
    void loadNewData();
    void selectFileRays();
    void selectFileC1();
    void selectFileC2();
    void selectFileC3();

};

#endif // LOADDATADIALOG_H

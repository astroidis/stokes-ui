#ifndef LOGFILEWIDGET_H
#define LOGFILEWIDGET_H

#include <QWidget>

namespace Ui {
class LogFileWidget;
}

class LogFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogFileWidget(QString file, QWidget *parent = nullptr);
    ~LogFileWidget();

private:
    Ui::LogFileWidget *ui;
    QString filename;

    void openFile();
};

#endif // LOGFILEWIDGET_H

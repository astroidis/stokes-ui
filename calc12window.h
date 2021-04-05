#ifndef CALC12WINDOW_H
#define CALC12WINDOW_H

#include <QWidget>

namespace Ui {
class Calc12Window;
}

class Calc12Window : public QWidget
{
    Q_OBJECT

public:
    explicit Calc12Window(QWidget *parent = nullptr);
    ~Calc12Window();

private:
    Ui::Calc12Window *ui;
};

#endif // CALC12WINDOW_H

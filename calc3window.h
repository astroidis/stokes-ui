#ifndef CALC3WINDOW_H
#define CALC3WINDOW_H

#include <QWidget>

namespace Ui {
class Calc3Window;
}

class Calc3Window : public QWidget
{
    Q_OBJECT

public:
    explicit Calc3Window(QWidget *parent = nullptr);
    ~Calc3Window();

private:
    Ui::Calc3Window *ui;
};

#endif // CALC3WINDOW_H

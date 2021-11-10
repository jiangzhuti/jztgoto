#ifndef MOTORCONTROLWIDGET_H
#define MOTORCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class MotorControlWidget;
}

class MotorControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MotorControlWidget(QWidget *parent = nullptr);
    ~MotorControlWidget();

private slots:
    void on_pb_start_clicked();

private:
    Ui::MotorControlWidget *ui;
};

#endif // MOTORCONTROLWIDGET_H

#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <optional>
#include <QWidget>
#include <QtGamepad/QGamepad>
#include "MotorController.h"

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();
    void set_enable(bool enable);

signals:
    void update_motor_status(MotorRole);

private:
    void update_ui();

private slots:
    void on_axis_left_x_change(double value);
    void on_axis_left_y_change(double value);
    void on_axis_right_x_change(double value);
    void on_axis_right_y_change(double value);
    void on_button_a_change(bool pressed);
    void on_button_b_change(bool pressed);
    void on_button_x_change(bool pressed);
    void on_button_y_change(bool pressed);
    void on_button_up_change(bool pressed);
    void on_button_down_change(bool pressed);
    void on_button_left_change(bool pressed);
    void on_button_right_change(bool pressed);
    void on_button_start_change(bool pressed);
    void on_button_r1_change(bool pressed);
    void on_button_r2_change(bool pressed);


    void on_pb_save_clicked();

    void on_pb_reload_clicked();

private:
    static constexpr double eps = 0.2;
    MotorRole motor_selected;
    std::optional<QGamepad> gamepad_opt;
    bool in_glance_mode;
    Ui::ControlWidget *ui;
};

#endif // CONTROLWIDGET_H

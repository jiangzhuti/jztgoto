#ifndef MOTORCONTROLWIDGET_H
#define MOTORCONTROLWIDGET_H

#include <QWidget>
#include "MotorController.h"

namespace Ui {
class MotorControlWidget;
}

class MotorControlWidget : public QWidget
{
    Q_OBJECT

    friend class ControlWidget;
public:
    explicit MotorControlWidget(QWidget *parent = nullptr);
    ~MotorControlWidget();

    void init(MotorRole r);
    void set_enabled(bool enable);
    bool get_enabled();
    void begin_glance(MotorDirection dir);
    void end_glance();
private:
    void pb_start_click();
    void pb_dir_plus_click();
    void pb_dir_minus_click();
    void pb_freq_inc_click();
    void pb_freq_dec_click();
    void sb_glance_inc();
    void sb_glance_dec();

signals:
    void update_motor_status();

private slots:
    void on_pb_start_clicked();
    void on_pb_dir_plus_clicked();

    void on_pb_dir_minus_clicked();

    void on_pb_freq_inc_clicked();

    void on_pb_freq_dec_clicked();
private:
    void paintEvent(QPaintEvent *);
    void update_ui();
private:
    bool enabled;
    bool in_glance;
    std::optional<Motor>* motor_opt;
    Ui::MotorControlWidget *ui;
};

#endif // MOTORCONTROLWIDGET_H

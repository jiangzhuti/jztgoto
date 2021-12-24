#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include "MotorController.h"
#include "MotorControlWidget.h"
#include "ui_MotorControlWidget.h"

MotorControlWidget::MotorControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorControlWidget)
{
    mode = MotorControlMode::TRACK;
    ui->setupUi(this);
}

void MotorControlWidget::init(MotorRole r)
{
    switch (r) {
    case MotorRole::DEC:
        motor_opt = &MotorController::dec;
        ui->labe_role->setText("DEC");
        break;
    case MotorRole::RA:
        motor_opt = &MotorController::ra;
        ui->labe_role->setText("RA");
        break;
    }
    if (!motor_opt->has_value()) {
        set_enabled(false);
    } else {
        set_enabled(true);
    }
    update_ui();
}

MotorControlWidget::~MotorControlWidget()
{
    delete ui;
}

void MotorControlWidget::begin_glance(MotorDirection dir)
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    if (mode == MotorControlMode::GLANCE) {
        if (motor.get_direction() == dir) {
            return;
        } else {
            motor.restore_status();
        }
    }
    double factor = ui->sb_glance->value();
    set_enabled(false);
    MotorStatus status = motor.get_status();
    status.enabled = true;
    status.dir = dir;
    status.freq *= factor;
    motor.apply_status(status);
    mode = MotorControlMode::GLANCE;
    update_ui();
}

void MotorControlWidget::end_glance()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    motor.restore_status();
    mode = MotorControlMode::TRACK;
    set_enabled(true);
    update_ui();
}

void MotorControlWidget::begin_rush(MotorDirection dir)
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    if (mode == MotorControlMode::RUSH) {
        if (motor.get_direction() == dir) {
            return;
        } else {
            motor.restore_status();
        }
    }
    double factor = ui->sb_glance->value();
    set_enabled(false);
    MotorStatus status = motor.get_status();
    status.enabled = true;
    status.dir = dir;
    status.freq *= factor;
    motor.apply_status(status);
    mode = MotorControlMode::RUSH;
    update_ui();
}

void MotorControlWidget::end_rush()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    motor.restore_status();
    mode = MotorControlMode::TRACK;
    set_enabled(true);
    update_ui();
}

void MotorControlWidget::set_enabled(bool enable)
{
    if (!motor_opt->has_value()) {
        enable = false;
    }
    enabled = enable;
    ui->pb_start->setEnabled(enable);
    ui->label_dir->setEnabled(enable);
    ui->pb_dir_minus->setEnabled(enable);
    ui->pb_dir_plus->setEnabled(enable);
    ui->label_freq->setEnabled(enable);
    ui->pb_freq_inc->setEnabled(enable);
    ui->pb_freq_dec->setEnabled(enable);
    ui->sb_glance->setEnabled(enable);
}

bool MotorControlWidget::get_enabled()
{
    return enabled;
}

void MotorControlWidget::pb_start_click()
{
    ui->pb_start->animateClick(50);
}

void MotorControlWidget::pb_dir_plus_click()
{
    ui->pb_dir_plus->animateClick(50);
}

void MotorControlWidget::pb_dir_minus_click()
{
    ui->pb_dir_minus->animateClick(50);
}

void MotorControlWidget::pb_freq_inc_click()
{
    ui->pb_freq_inc->animateClick(50);
}

void MotorControlWidget::pb_freq_dec_click()
{
    ui->pb_freq_dec->animateClick(50);
}

void MotorControlWidget::sb_glance_inc()
{
    ui->sb_glance->stepUp();
}

void MotorControlWidget::sb_glance_dec()
{
    ui->sb_glance->stepDown();
}

void MotorControlWidget::sb_rush_inc()
{
    ui->sb_rush->stepUp();
}

void MotorControlWidget::sb_rush_dec()
{
    ui->sb_rush->stepDown();
}

void MotorControlWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void MotorControlWidget::update_ui()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    ui->pb_start->setText(motor.get_enable() ? "stop" : "start");

    emit update_motor_status();
}


void MotorControlWidget::on_pb_start_clicked()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    motor.toggle_enable();
    update_ui();
}


void MotorControlWidget::on_pb_dir_plus_clicked()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    motor.set_direction(MotorDirection::PLUS);
    update_ui();
}



void MotorControlWidget::on_pb_dir_minus_clicked()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    motor.set_direction(MotorDirection::MINUS);
    update_ui();
}


void MotorControlWidget::on_pb_freq_inc_clicked()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    motor.freq_inc();
    update_ui();
}


void MotorControlWidget::on_pb_freq_dec_clicked()
{
    if (!motor_opt->has_value()) {
        return;
    }
    auto& motor = motor_opt->value();
    motor.freq_dec();
    update_ui();
}



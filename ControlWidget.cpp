#include <QLoggingCategory>
#include <QDebug>
#include "ControlWidget.h"
#include "ui_ControlWidget.h"


ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
    run_mode = MotorControlMode::TRACK;
    select_mode = MotorControlMode::GLANCE;
    motor_selected = MotorRole::RA;
    ui->widget_ctl_dec->init(MotorRole::DEC);
    ui->widget_ctl_ra->init(MotorRole::RA);
    connect(ui->widget_ctl_dec, &MotorControlWidget::update_motor_status, this, [this]() {emit update_motor_status(MotorRole::DEC);});
    connect(ui->widget_ctl_ra, &MotorControlWidget::update_motor_status, this, [this]() {emit update_motor_status(MotorRole::RA);});

    update_ui();

    qputenv("QT_GAMEPAD", "sdl2");
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
        qWarning() << "Did not find any connected gamepads";
    } else {
        gamepad_opt.emplace(*gamepads.begin());
        QGamepad* gamepad = &gamepad_opt.value();
        connect(gamepad, &QGamepad::axisLeftXChanged, this, &ControlWidget::on_axis_left_x_change);
        connect(gamepad, &QGamepad::axisLeftYChanged, this, &ControlWidget::on_axis_left_y_change);
        connect(gamepad, &QGamepad::axisRightXChanged, this, &ControlWidget::on_axis_right_x_change);
        connect(gamepad, &QGamepad::axisRightYChanged, this, &ControlWidget::on_axis_right_y_change);
        connect(gamepad, &QGamepad::buttonAChanged, this, &ControlWidget::on_button_a_change);
        connect(gamepad, &QGamepad::buttonBChanged, this, &ControlWidget::on_button_b_change);
        connect(gamepad, &QGamepad::buttonXChanged, this, &ControlWidget::on_button_x_change);
        connect(gamepad, &QGamepad::buttonYChanged, this, &ControlWidget::on_button_y_change);
        connect(gamepad, &QGamepad::buttonUpChanged, this, &ControlWidget::on_button_up_change);
        connect(gamepad, &QGamepad::buttonDownChanged, this, &ControlWidget::on_button_down_change);
        connect(gamepad, &QGamepad::buttonLeftChanged, this, &ControlWidget::on_button_left_change);
        connect(gamepad, &QGamepad::buttonRightChanged, this, &ControlWidget::on_button_right_change);
        connect(gamepad, &QGamepad::buttonStartChanged, this, &ControlWidget::on_button_start_change);
        connect(gamepad, &QGamepad::buttonR1Changed, this, &ControlWidget::on_button_r1_change);
        connect(gamepad, &QGamepad::buttonR2Changed, this, &ControlWidget::on_button_r2_change);
    }
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::set_enable(bool enable)
{
    ui->pb_save->setEnabled(enable);
    ui->pb_reload->setEnabled(enable);
    ui->widget_ctl_dec->set_enabled(enable);
    ui->widget_ctl_ra->set_enabled(enable);
}

void ControlWidget::update_ui()
{
    if (motor_selected == MotorRole::RA) {
        ui->widget_ctl_ra->setStyleSheet("MotorControlWidget { margin: 5px; border: 2px solid green; background-color: white; }");
        ui->widget_ctl_dec->setStyleSheet("");
        return;
    }
    if (motor_selected == MotorRole::DEC) {
        ui->widget_ctl_ra->setStyleSheet("");
        ui->widget_ctl_dec->setStyleSheet("MotorControlWidget { margin: 5px; border: 2px solid green; background-color: white; }");
        return;
    }
}

void ControlWidget::on_axis_left_x_change(double value)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (value > 0.0 + eps) {
        // select dec
        if (motor_selected == MotorRole::DEC) {
            return;
        }
        motor_selected = MotorRole::DEC;
        update_ui();
        return;
    }
    if (value < 0.0 - eps) {
        // select ra
        if (motor_selected == MotorRole::RA) {
            return;
        }
        motor_selected = MotorRole::RA;
        update_ui();
        return;
    }
}

void ControlWidget::on_axis_left_y_change(double value)
{

}

void ControlWidget::on_axis_right_x_change(double value)
{

}

void ControlWidget::on_axis_right_y_change(double value)
{
    if (value >= (0.0 - eps) && value <= (0.0 + eps)) {
        if (run_mode == MotorControlMode::TRACK) {
            return;
        }
        run_mode = MotorControlMode::TRACK;
        if (motor_selected == MotorRole::RA) {
            ui->widget_ctl_ra->end_glance();
        } else {
            ui->widget_ctl_dec->end_glance();
        }
        qDebug() << __func__ << "@ " << __LINE__ << ": end glance, axis value = " << value;
        set_enable(true);
        update_ui();
        emit update_control_mode_status(run_mode, select_mode);
        return;
    }
    if (value > 0.0 + eps) {
        if (select_mode == MotorControlMode::GLANCE || select_mode == MotorControlMode::RUSH) {
            run_mode = select_mode;
        } else {
            qFatal("select mode error");
        }
        if (motor_selected == MotorRole::RA) {
            if (run_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_ra->begin_glance(MotorDirection::MINUS);
            } else if (run_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_ra->begin_rush(MotorDirection::MINUS);
            }
        } else {
            if (run_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_dec->begin_glance(MotorDirection::MINUS);
            } else if (run_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_dec->begin_rush(MotorDirection::MINUS);
            }
        }
        qDebug() << __func__ << "@ " << __LINE__ << ": begin glance plus, axis value = " << value;
        set_enable(false);
        update_ui();
        emit update_control_mode_status(run_mode, select_mode);
        return;
    }
    if (value < 0.0 - eps) {
        if (select_mode == MotorControlMode::GLANCE || select_mode == MotorControlMode::RUSH) {
            run_mode = select_mode;
        } else {
            qFatal("select mode error");
        }
        if (motor_selected == MotorRole::RA) {
            if (run_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_ra->begin_glance(MotorDirection::PLUS);
            } else if (run_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_ra->begin_rush(MotorDirection::PLUS);
            }
        } else {
            if (run_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_dec->begin_glance(MotorDirection::PLUS);
            } else if (run_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_dec->begin_rush(MotorDirection::PLUS);
            }
        }
        qDebug() << __func__ << "@ " << __LINE__ << ": begin glance minus, axis value = " << value      ;
        set_enable(false);
        update_ui();
        emit update_control_mode_status(run_mode, select_mode);
        return;
    }
}

void ControlWidget::on_button_a_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }
    if (pressed) {
        if (motor_selected == MotorRole::RA) {
            if (select_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_ra->sb_glance_dec();
            } else if (select_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_ra->sb_rush_dec();
            }
            return;
        }
        if (motor_selected == MotorRole::DEC) {
            if (select_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_dec->sb_glance_dec();
            } else if (select_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_dec->sb_rush_dec();
            }            return;
        }
    }
}

void ControlWidget::on_button_b_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }
    if (pressed) {
        if (motor_selected == MotorRole::RA) {
            if (select_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_ra->sb_glance_inc();
            } else if (select_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_ra->sb_rush_inc();
            }
            return;
        }
        if (motor_selected == MotorRole::DEC) {
            if (select_mode == MotorControlMode::GLANCE) {
                ui->widget_ctl_dec->sb_glance_inc();
            } else if (select_mode == MotorControlMode::RUSH) {
                ui->widget_ctl_dec->sb_rush_inc();
            }            return;
        }
    }
}

void ControlWidget::on_button_x_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }
    if (select_mode == MotorControlMode::GLANCE) {
        return;
    }
    if (pressed) {
        select_mode = MotorControlMode::GLANCE;
        emit update_control_mode_status(run_mode, select_mode);
    }
}

void ControlWidget::on_button_y_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }
    if (select_mode == MotorControlMode::RUSH) {
        return;
    }
    if (pressed) {
        select_mode = MotorControlMode::RUSH;
        emit update_control_mode_status(run_mode, select_mode);
    }
}

void ControlWidget::on_button_up_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (pressed) {
        if (motor_selected == MotorRole::RA) {
            ui->widget_ctl_ra->pb_freq_inc_click();
            return;
        }
        if (motor_selected == MotorRole::DEC) {
            ui->widget_ctl_dec->pb_freq_inc_click();
            return;
        }
    }
}

void ControlWidget::on_button_down_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (pressed) {
        if (motor_selected == MotorRole::RA) {
            ui->widget_ctl_ra->pb_freq_dec_click();
            return;
        }
        if (motor_selected == MotorRole::DEC) {
            ui->widget_ctl_dec->pb_freq_dec_click();
            return;
        }
    }
}

void ControlWidget::on_button_left_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (pressed) {
        if (motor_selected == MotorRole::RA) {
            ui->widget_ctl_ra->pb_dir_minus_click();
            return;
        }
        if (motor_selected == MotorRole::DEC) {
            ui->widget_ctl_dec->pb_dir_minus_click();
            return;
        }
    }
}

void ControlWidget::on_button_right_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (pressed) {
        if (motor_selected == MotorRole::RA) {
            ui->widget_ctl_ra->pb_dir_plus_click();
            return;
        }
        if (motor_selected == MotorRole::DEC) {
            ui->widget_ctl_dec->pb_dir_plus_click();
            return;
        }
    }
}

void ControlWidget::on_button_start_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (pressed) {
        if (motor_selected == MotorRole::RA) {
            ui->widget_ctl_ra->pb_start_click();
            return;
        }
        if (motor_selected == MotorRole::DEC) {
            ui->widget_ctl_dec->pb_start_click();
            return;
        }
    }
}

void ControlWidget::on_button_r1_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (pressed){
        if (gamepad_opt.has_value()) {
            auto& gamepad = gamepad_opt.value();
            if (gamepad.buttonSelect()) {
                ui->pb_save->animateClick(50);
            }
        }
    }
}

void ControlWidget::on_button_r2_change(bool pressed)
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (pressed){
        if (gamepad_opt.has_value()) {
            auto& gamepad = gamepad_opt.value();
            if (gamepad.buttonSelect()) {
                ui->pb_reload->animateClick(50);
            }
        }
    }
}

void ControlWidget::on_pb_save_clicked()
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (MotorController::ra.has_value()) {
        MotorController::ra->save();
    }
    if (MotorController::dec.has_value()) {
        MotorController::dec->save();
    }
}


void ControlWidget::on_pb_reload_clicked()
{
    if (run_mode != MotorControlMode::TRACK) {
        return;
    }

    if (MotorController::ra.has_value()) {
        MotorController::ra->reload();
        emit update_motor_status(MotorRole::RA);
        ui->widget_ctl_ra->update_ui();
    }
    if (MotorController::dec.has_value()) {
        MotorController::dec->reload();
        emit update_motor_status(MotorRole::DEC);
        ui->widget_ctl_dec->update_ui();
    }
}


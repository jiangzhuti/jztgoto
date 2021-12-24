#include "MotorController.h"

#include "StatusWidget.h"
#include "ui_StatusWidget.h"

static void set_qlabel_active(QLabel* lb)
{
    lb->setStyleSheet("");
    lb->setStyleSheet("QLabel {border: 2px solid green; color: OrangeRed ; font-weight:bold; background-color: white; }");
}

static void set_qlabel_select(QLabel* lb)
{
    lb->setStyleSheet("");
    lb->setStyleSheet("QLabel {border: 2px dotted blue; }");
}

static void set_qlabel_normal(QLabel* lb)
{
    lb->setStyleSheet("");
}

static void set_qlabel_disable(QLabel* lb)
{
    lb->setStyleSheet("");
    lb->setStyleSheet("color: LightGrey; ");
}

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    update_status(MotorRole::RA);
    update_status(MotorRole::DEC);

    update_control_mode_status(MotorControlMode::TRACK, MotorControlMode::GLANCE);
}

StatusWidget::~StatusWidget()
{
    delete ui;
}


void StatusWidget::update_status(MotorRole role)
{
    QString status;
    if (role == MotorRole::RA) {
        status.append(QString("%1").arg("RA:", -4));
        if (MotorController::ra) {
            status.append(MotorController::ra->get_status_string());
        } else {
            status.append("N/A");
        }
        ui->label_status_ra->setText(status);
        return;
    }
    if (role == MotorRole::DEC) {
        status.append(QString("%1").arg("DEC:", -4));
        if (MotorController::dec) {
            status.append(MotorController::dec->get_status_string());
        } else {
            status.append("N/A");
        }
        ui->label_status_dec->setText(status);
        return;
    }
}

void StatusWidget::update_control_mode_status(MotorControlMode active_mode, MotorControlMode select_mode)
{
    if (select_mode == MotorControlMode::GLANCE) {
        set_qlabel_normal(ui->label_rush);
        set_qlabel_select(ui->label_glance);
    } else if (select_mode == MotorControlMode::RUSH) {
        set_qlabel_normal(ui->label_glance);
        set_qlabel_normal(ui->label_rush);
    }
    if (active_mode == MotorControlMode::TRACK) {
        set_qlabel_active(ui->label_track);
    } else {
        set_qlabel_normal(ui->label_track);
    }
    if (active_mode == MotorControlMode::GLANCE) {
        set_qlabel_active(ui->label_glance);
        set_qlabel_disable(ui->label_track);
        set_qlabel_disable(ui->label_rush);
    }
    if (active_mode == MotorControlMode::RUSH) {
        set_qlabel_active(ui->label_rush);
        set_qlabel_disable(ui->label_track);
        set_qlabel_disable(ui->label_glance);
    }

}

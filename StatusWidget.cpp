#include "MotorController.h"

#include "StatusWidget.h"
#include "ui_StatusWidget.h"

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);
    update_status(MotorRole::RA);
    update_status(MotorRole::DEC);
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

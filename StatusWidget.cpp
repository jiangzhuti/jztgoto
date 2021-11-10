#include "MotorController.h"

#include "StatusWidget.h"
#include "ui_StatusWidget.h"

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);
    update_status();
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::update_status()
{
    QString status;
    status.append(QString("%1").arg("RA:", -5));
    if (MotorController::ra) {
        status.append(MotorController::ra->get_status());
    } else {
        status.append("N/A");
    }
    status.append("\n");
    status.append(QString("%1").arg("DEC:", -5));
    if (MotorController::dec) {
        status.append(MotorController::dec->get_status());
    } else {
        status.append("N/A");
    }
    ui->label_status->setText(status);
}

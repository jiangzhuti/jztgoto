#include "MotorControlWidget.h"
#include "ui_MotorControlWidget.h"

MotorControlWidget::MotorControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorControlWidget)
{
    ui->setupUi(this);
}

MotorControlWidget::~MotorControlWidget()
{
    delete ui;
}

void MotorControlWidget::on_pb_start_clicked()
{

}


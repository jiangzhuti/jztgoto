#include <QDebug>
#include "Widget.h"
#include "ui_Widget.h"

#include "ControlWidget.h"
#include "StatusWidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->widget_control, &ControlWidget::update_motor_status, ui->widget_status, &StatusWidget::update_status);
}

Widget::~Widget()
{
    delete ui;
}


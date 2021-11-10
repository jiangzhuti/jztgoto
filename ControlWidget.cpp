#include <QtGamepad/QGamepad>
#include <QDebug>
#include "ControlWidget.h"
#include "ui_ControlWidget.h"


ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
        qWarning() << "Did not find any connected gamepads";
    }

    ui->setupUi(this);
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

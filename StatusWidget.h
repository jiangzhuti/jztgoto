#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>
#include "MotorController.h"

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = nullptr);
    ~StatusWidget();

public slots:
    void update_status(MotorRole role);
    void update_control_mode_status(MotorControlMode active_mode, MotorControlMode select_mode);


private:
    Ui::StatusWidget *ui;

};

#endif // STATUSWIDGET_H

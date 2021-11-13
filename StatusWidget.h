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

private:
    Ui::StatusWidget *ui;
};

#endif // STATUSWIDGET_H

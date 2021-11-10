#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

private:
    void show_ip();
    void show_time();
    void show_cpu_temp();

private slots:
    void update_info();

private:
    Ui::InfoWidget *ui;
};

#endif // INFOWIDGET_H

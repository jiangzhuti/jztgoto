#include <QTimer>
#include <QDateTime>
#include <QNetworkInterface>
#include <QFile>

#include "InfoWidget.h"
#include "ui_InfoWidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &InfoWidget::update_info);
    timer->start(1000);

    update_info();
}

void InfoWidget::show_time()
{
    auto dt = QDateTime::currentDateTime();
    ui->label_clock_val->setText(dt.toString("yyyy-MM-dd hh:mm:ss"));
}

void InfoWidget::show_ip()
{
    auto ifs = QNetworkInterface::allInterfaces();
    for (auto& i : ifs) {
        if (i.type() != QNetworkInterface::Wifi) continue;
        auto addrs = i.addressEntries();
        for (auto& addr : addrs) {
            if (addr.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                ui->label_ip_val->setText(addr.ip().toString());
                return;
            }
        }
    }
    ui->label_ip_val->setText("No address");
}

void InfoWidget::show_cpu_temp()
{
    QFile thermal_file("/sys/class/thermal/thermal_zone0/temp");
    if (thermal_file.open(QIODevice::ReadOnly)) {
        QTextStream in(&thermal_file);
        QString line = in.readLine();
        float temp = line.toFloat() / 1000;
        ui->label_cpu_temp_val->setText(QString("%1°C").arg(temp));
        thermal_file.close();
    } else {
        ui->label_cpu_temp_val->setText("N/A");
    }
}

void InfoWidget::update_info()
{
    show_time();
    show_ip();
    show_cpu_temp();
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

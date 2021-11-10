#include <QDebug>
#include <pigpio.h>

#include "MotorController.h"

#define DEFAULT_DUTY 500000

Motor::Motor(const MotorConfig& c) : cfg(c)
{
    gpioSetMode(cfg.enc_port, PI_OUTPUT);
    gpioSetMode(cfg.dir_port, PI_OUTPUT);
    gpioWrite(cfg.enc_port, PI_ON);
    gpioWrite(cfg.dir_port, cfg.dir == MotorDirection::PLUS ? PI_ON : PI_OFF);
    gpioHardwarePWM(cfg.pulse_port, cfg.base_freq, DEFAULT_DUTY);

    status.enabled = false;
    status.level = 1;
    status.dir = cfg.dir;
    status.freq = cfg.base_freq;
}

Motor::~Motor()
{
    gpioHardwarePWM(cfg.pulse_port, 1, 0);
    set_enable(false);
}

void Motor::set_enable(bool enable)
{
    if (enable == status.enabled) return;
    gpioWrite(cfg.enc_port, enable ? PI_OFF : PI_ON);
    status.enabled = enable;
}

void Motor::set_level(unsigned level)
{
    if (level > cfg.max_level) return;
    if (level < 1) return;
    unsigned new_freq = cfg.base_freq * level;
    status.level = level;
    status.freq = new_freq;
    gpioHardwarePWM(cfg.pulse_port, new_freq, DEFAULT_DUTY);
}

void Motor::level_inc()
{
    if (status.level == cfg.max_level) {
        return;
    }
    status.level++;
    status.freq += cfg.base_freq;
    gpioHardwarePWM(cfg.pulse_port, status.freq, DEFAULT_DUTY);
}

void Motor::level_dec()
{
    if (status.level == 1) {
        return;
    }
    status.level--;
    status.freq -= cfg.base_freq;
    gpioHardwarePWM(cfg.pulse_port, status.freq, DEFAULT_DUTY);
}

void Motor::set_direction(MotorDirection dir)
{
    if (dir == status.dir) return;
    gpioWrite(cfg.dir_port, dir == MotorDirection::PLUS ? PI_ON : PI_OFF);
    status.dir = dir;
}

QString Motor::get_status()
{
    return status.to_string();
}

std::optional<Motor> MotorController::ra;
std::optional<Motor> MotorController::dec;

void MotorController::init(const nlohmann::json &j)
{
    try {
        MotorConfig ra_cfg, dec_cfg;
        auto& j_motors = j["motors"];
        if (j_motors.contains("ra")) {
            auto& j_ra = j_motors["ra"];
            ra_cfg.enc_port = j_ra["enc_port"];
            ra_cfg.dir_port = j_ra["dir_port"];
            ra_cfg.base_freq = j_ra["base_freq"];
            ra_cfg.max_level = j_ra["max_level"];
            ra.emplace(ra_cfg);
        }
        if (j_motors.contains("dec")) {
            auto& j_dec = j_motors["dec"];
            dec_cfg.enc_port = j_dec["enc_port"];
            dec_cfg.dir_port = j_dec["dir_port"];
            dec_cfg.base_freq = j_dec["base_freq"];
            dec_cfg.max_level = j_dec["max_level"];
            dec.emplace(dec_cfg);
        }
    } catch (nlohmann::json::exception& e) {
        qFatal("%s", e.what());
    }
}

/*
QString MotorController::get_status()
{
    QString status;
    status.append(QString("%1").arg("RA:", -5));
    if (ra) {
        status.append(ra->get_status());
    } else {
        status.append("N/A");
    }
    status.append("\n");
    status.append(QString("%1").arg("DEC:", -5));
    if (dec) {
        status.append(dec->get_status());
    } else {
        status.append("N/A");
    }
    return status;
}
*/

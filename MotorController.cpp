#include <QDebug>
#include <pigpio.h>

#include "MotorController.h"

static inline unsigned half_duty(unsigned freq)
{
    unsigned period = 1e9 / freq;
    return period / 2;
}

Motor::Motor(const MotorConfig& c) : cfg(c)
{
    gpioSetMode(cfg.enc_port, PI_OUTPUT);
    gpioSetMode(cfg.dir_port, PI_OUTPUT);
    gpioWrite(cfg.enc_port, PI_ON);
    gpioWrite(cfg.dir_port, cfg.dir == MotorDirection::PLUS ? PI_ON : PI_OFF);
    gpioHardwarePWM(cfg.pulse_port, cfg.base_freq, half_duty(cfg.base_freq));

    status.enabled = false;
    status.dir = cfg.dir;
    status.freq = cfg.base_freq;

    status_shadow = status;
}

Motor::~Motor()
{
    gpioHardwarePWM(cfg.pulse_port, 1, 0);
    set_enable(false);
}

void Motor::do_kc(unsigned kc_freq, unsigned kc_time)
{
    gpioHardwarePWM(cfg.pulse_port, kc_freq, half_duty(kc_freq));
    gpioWrite(cfg.enc_port, PI_OFF);
    gpioDelay(kc_time * 1000);
    gpioHardwarePWM(cfg.pulse_port, status.freq, half_duty(status.freq));
}

void Motor::set_enable(bool enable)
{
    if (enable == status.enabled) return;
    if (enable) {
        do_kc(cfg.kc_freq, cfg.kc_time);
        gpioHardwarePWM(cfg.pulse_port, status.freq, half_duty(status.freq));
    } else {
        gpioWrite(cfg.enc_port, PI_ON);
    }
    status.enabled = enable;
    status_shadow = status;
}

void Motor::toggle_enable()
{
    set_enable(!status.enabled);
}

bool Motor::get_enable()
{
    return status.enabled;
}

void Motor::freq_inc(unsigned step)
{
    status.freq += step;
    gpioHardwarePWM(cfg.pulse_port, status.freq, half_duty(status.freq));
    status_shadow = status;
}

void Motor::freq_dec(unsigned step)
{

    if (status.freq < step) {
        status.freq = 0;
    } else {
        status.freq -= step;
    }
    gpioHardwarePWM(cfg.pulse_port, status.freq, half_duty(status.freq));
    status_shadow = status;
    return;
}

void Motor::set_freq(unsigned int new_freq)
{
    status.freq = new_freq;
    gpioHardwarePWM(cfg.pulse_port, status.freq, half_duty(status.freq));
    status_shadow = status;
}

unsigned Motor::get_freq()
{
    return status.freq;
}

void Motor::set_direction(MotorDirection dir)
{
    if (dir == status.dir) return;
    gpioWrite(cfg.dir_port, dir == MotorDirection::PLUS ? PI_ON : PI_OFF);
    status.dir = dir;
    if (status.enabled) {
        do_kc(cfg.kc_freq, cfg.kc_time * 2);
    }
    status_shadow = status;
}

MotorDirection Motor::get_direction()
{
    return status.dir;
}

QString Motor::get_status_string()
{
    return status.to_string();
}

MotorStatus Motor::get_status()
{
    return status;
}

void Motor::apply_status(MotorStatus& new_status)
{
    status_shadow = status;
    status = new_status;
    gpioWrite(cfg.enc_port, PI_ON);
    gpioDelay(10 * 1000);
    gpioWrite(cfg.dir_port, status.dir == MotorDirection::PLUS ? PI_ON : PI_OFF);
    gpioHardwarePWM(cfg.pulse_port, status.freq, half_duty(status.freq));
    gpioWrite(cfg.enc_port, status.enabled ? PI_OFF : PI_ON);
}

void Motor::restore_status()
{
    if (status_shadow == status) return;
    status = status_shadow;
    apply_status(status);
}

void Motor::save()
{
    cfg.base_freq = status.freq;
    cfg.dir = status.dir;
    cfg.enabled = status.enabled;
}

void Motor::reload()
{
    set_enable(false);
    set_direction(cfg.dir);
    set_freq(cfg.base_freq);
    set_enable(cfg.enabled);
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
            ra_cfg.pulse_port = j_ra["pulse_port"];
            ra_cfg.base_freq = j_ra["base_freq"];
            ra_cfg.kc_freq = j_ra["kc_freq"];
            ra_cfg.kc_time = j_ra["kc_time"];
            if (j_ra.contains("dir")) {
                if (j_ra["dir"] == '+') {
                    ra_cfg.dir = MotorDirection::PLUS;
                } else if (j_ra["dir"] == '-') {
                    ra_cfg.dir = MotorDirection::MINUS;
                }
            }
            ra.emplace(ra_cfg);
        }
        if (j_motors.contains("dec")) {
            auto& j_dec = j_motors["dec"];
            dec_cfg.enc_port = j_dec["enc_port"];
            dec_cfg.dir_port = j_dec["dir_port"];
            dec_cfg.pulse_port = j_dec["pulse_port"];
            dec_cfg.base_freq = j_dec["base_freq"];
            dec_cfg.kc_freq = j_dec["kc_freq"];
            dec_cfg.kc_time = j_dec["kc_time"];
            if (j_dec.contains("dir")) {
                if (j_dec["dir"] == "+") {
                    dec_cfg.dir = MotorDirection::PLUS;
                } else if (j_dec["dir"] == "-") {
                    dec_cfg.dir = MotorDirection::MINUS;
                }
            }
            dec.emplace(dec_cfg);
        }
    } catch (nlohmann::json::exception& e) {
        qFatal("%s", e.what());
    }
}

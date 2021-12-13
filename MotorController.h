#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <QObject>
#include <optional>
#include <nlohmann/json.hpp>

enum class MotorDirection {
    PLUS,
    MINUS
};

enum class MotorRole {
    DEC,
    RA
};

struct MotorConfig {
    unsigned enc_port;
    unsigned dir_port;
    unsigned pulse_port;
    unsigned base_freq;
    MotorDirection dir = MotorDirection::PLUS;
    unsigned kc_freq;
    unsigned kc_time;
    bool enabled = false;
};

struct MotorStatus {
    bool enabled = false;
    MotorDirection dir = MotorDirection::PLUS;
    unsigned freq = 0;

    QString to_string()
    {
        return QString("%1 Dir: %3 Freq: %4")
                .arg(enabled ? "UP" : "DOWN", -4)
                .arg(dir == MotorDirection::PLUS ? "+" : "-", -1)
                .arg(freq, -2);
    }
    bool operator == (const MotorStatus& o) const
    {
        return enabled == o.enabled && dir == o.dir && freq == o.freq;
    }
};

class Motor {
public:
    Motor(const MotorConfig& c);
    ~Motor();
    void set_enable(bool enable);
    void toggle_enable();
    bool get_enable();
    void freq_inc(unsigned step = 1);
    void freq_dec(unsigned step = 1);
    void set_freq(unsigned new_freq);
    unsigned get_freq();
    void set_direction(MotorDirection dir);
    void toggle_direction();
    MotorDirection get_direction();
    QString get_status_string();
    MotorStatus get_status();
    void apply_status(MotorStatus& new_status);
    void restore_status();

    void save();
    void reload();
private:
    static constexpr unsigned smooth_delta_freq = 500;
    static constexpr unsigned smooth_delta_time = 50;

    void change_freq(unsigned new_freq);
    void change_enable(bool enable, bool kc);
    void change_direction(MotorDirection dir);
    MotorConfig cfg;
    MotorStatus status;
    MotorStatus status_shadow;
};

class MotorController : public QObject
{
    Q_OBJECT
    MotorController() = delete;
    MotorController(const MotorController&) = delete;
    MotorController(MotorController&&) = delete;
public:
    static void init(const nlohmann::json& j);
    static std::optional<Motor> ra;
    static std::optional<Motor> dec;
};



#endif // MOTORCONTROLLER_H

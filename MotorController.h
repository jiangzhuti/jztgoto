#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <QObject>
#include <optional>
#include <nlohmann/json.hpp>

enum class MotorDirection {
    PLUS,
    MINUS
};

struct MotorConfig {
    unsigned enc_port;
    unsigned dir_port;
    unsigned pulse_port;
    unsigned base_freq;
    unsigned max_level;
    MotorDirection dir;
};

struct MotorStatus {
    bool enabled = false;
    unsigned level = 1;
    MotorDirection dir = MotorDirection::PLUS;
    unsigned freq = 0;
    QString to_string()
    {
        return QString("%1 Direction: %2 Level: %3").arg(enabled ? "enabled" : "disabled", -8)
                                  .arg(dir == MotorDirection::PLUS ? "+" : "-", -2)
                                  .arg(level, -2);
    }
};

class Motor {
public:
    Motor(const MotorConfig& c);
    ~Motor();
    void set_enable(bool enable);
    void set_level(unsigned level);
    void level_inc();
    void level_dec();
    void set_direction(MotorDirection dir);
    QString get_status();
private:
    MotorConfig cfg;
    MotorStatus status;
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

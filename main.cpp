#include <fstream>
#include <QCommandLineParser>
#include "GPIO.h"
#include "Widget.h"
#include "MotorController.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("jztgoto");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("a goto system developed by jzt");
    parser.addHelpOption();
    parser.addVersionOption();


    QCommandLineOption conf_file_opt(QStringList() << "c" << "conf", QCoreApplication::translate("main", "config file"),
                                     QCoreApplication::translate("main", "conf-file"),
                                     "jztgoto.json");
    parser.addOption(conf_file_opt);
    parser.process(app);
    QString conf_file = parser.value(conf_file_opt);
    nlohmann::json j;
    try {
        std::ifstream file(conf_file.toStdString());
        if (!file.is_open()) {
            qFatal("cannot open file %s", conf_file.toStdString().c_str());
        }
        j = nlohmann::json::parse(file);
        MotorController::init(j);
    }  catch (std::exception& e) {
        qFatal("%s", e.what());
    }

    gpio_init();

    Widget w;
    w.show();
    auto ret = app.exec();
    gpio_cleanup();
    return ret;
}

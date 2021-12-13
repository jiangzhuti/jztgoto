QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network gamepad texttospeech

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ControlWidget.cpp \
    GPIO.cpp \
    InfoWidget.cpp \
    MotorControlWidget.cpp \
    MotorController.cpp \
    StatusWidget.cpp \
    main.cpp \
    Widget.cpp

HEADERS += \
    ControlWidget.h \
    GPIO.h \
    InfoWidget.h \
    MotorControlWidget.h \
    MotorController.h \
    StatusWidget.h \
    Widget.h

FORMS += \
    ControlWidget.ui \
    InfoWidget.ui \
    MotorControlWidget.ui \
    StatusWidget.ui \
    Widget.ui

LIBS += -lpigpio -lm

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    jztgoto.json

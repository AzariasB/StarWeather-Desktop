#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T08:59:52
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeatherStation
TEMPLATE = app

win32:RC_ICONS += icon.ico

DEFINES += QT_DEPRECATED_WARNINGS

# For std clamp mainly
CONFIG += c++17

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    SensorsGraph.cpp \
    SensorValue.cpp \
    ImprovedSerial.cpp

HEADERS += \
        MainWindow.hpp \
    SensorsGraph.hpp \
    SensorValue.hpp \
    ImprovedSerial.hpp \
    Config.hpp

FORMS += \
        MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

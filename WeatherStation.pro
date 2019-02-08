#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T08:59:52
#
#-------------------------------------------------

QT       += core gui serialport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeatherStation
TEMPLATE = app

win32:RC_ICONS += icon.ico
macx {
    ICON = icon.icns
}

DEFINES += QT_DEPRECATED_WARNINGS

# For std clamp mainly
CONFIG += c++17

SOURCES += \
        src/main.cpp \
        src/MainWindow.cpp \
    src/SensorValue.cpp \
    src/ImprovedSerial.cpp \
    src/SensorChart.cpp

HEADERS += \
        src/MainWindow.hpp \
    src/SensorValue.hpp \
    src/ImprovedSerial.hpp \
    src/Config.hpp \
    src/SensorChart.hpp

FORMS += \
        src/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

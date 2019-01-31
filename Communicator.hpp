/*
 * The MIT License
 *
 * Copyright 2017-2018 azarias.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * File:   Communicator.hpp
 * Author: azarias
 *
 * Created on 15/12/2018
 */
#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <qtypeinfo.h>
#include <QObject>
#include <QSerialPort>
#include <QQueue>

constexpr quint16 VALUE_MASK     = 0b0000001111111111;
constexpr quint16 SENSORID_MASK  = 0b0000110000000000;
constexpr quint16 FREQUENCY_MASK = 0b1111000000000000;

enum WeatherCommand : quint8 {
    STOP_MODE = 0x0,
    START_MODE_1 = 0x1,
    START_MODE_2 = 0x2,
    START_MODE_3 = 0x3,
    GET_DATA = 0x4,
    CONFIGURE_FE_1 = 0x5,
    CONFIGURE_FE_2 = 0x6,
    CONFIGURE_FE_3 = 0x7,
    CONFIGURE_MODE_2 = 0x8,
    SEND_MODE1_DATA = 0x9,
    SEND_MODE2_DATA = 0xA
};

struct SensorValue{
    qint8 sensorId;
    qint16 value;
    qint8 frequency;
};

class Communicator : public QObject
{
    Q_OBJECT
public:
    Communicator(QSerialPort &port, QObject *parent = nullptr);

    bool connectTo(const QString &serialPort);

    bool sendCommand(WeatherCommand command, qint8 argument = 0);

    QVector<qint16> getMode3Data();
signals:
    void receivedValue(SensorValue val);

    void receivedPack(QVector<SensorValue> values);

    void confirmCommand(WeatherCommand command, qint8 exeCode);

private slots:
    void readSerial();

private:
    QSerialPort &m_port;

    SensorValue toSensorValue(char d1, char d2);

    QVector<SensorValue> readPack(QQueue<char> &queue, quint16 size);

    quint16 toSize(char byte1, char byte2);



    void parseCommand(QQueue<char> &queue);
};

#endif // COMMUNICATOR_HPP

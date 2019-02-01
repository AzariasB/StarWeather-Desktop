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
#include "SensorValue.hpp"

class Communicator : public QObject
{
    Q_OBJECT
public:
    Communicator(QSerialPort &port, QObject *parent = nullptr);

    bool connectTo(const QString &serialPort);

    bool sendCommand(WeatherCommand command, qint8 argument = 0);

    quint8 currentMode() const;

    QVector<qint16> getMode3Data();
signals:
    void receivedValue(SensorValue val);

    void receivedPack(QVector<SensorValue> values);

    void receivedFrequencies(quint8 interval1, quint8 interval2, quint8 interval3, quint8 mode2Freq);

    void confirmCommand(WeatherCommand command, qint8 exeCode);

private slots:
    void readSerial();

private:
    QSerialPort &m_port;

    QVector<SensorValue> readPack(QQueue<char> &queue, quint16 size);

    quint16 toSize(char byte1, char byte2);

    bool m_started = false;

    WeatherCommand m_currentMode = WeatherCommand::STOP_START_MODE;

    void parseCommand(QQueue<char> &queue);
};

#endif // COMMUNICATOR_HPP

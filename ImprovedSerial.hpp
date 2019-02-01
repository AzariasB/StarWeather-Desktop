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
 * File:   ImprovedSerial.hpp
 * Author: azarias
 *
 * Created on 1/2/2019
 */
#ifndef IMPROVEDSERIAL_HPP
#define IMPROVEDSERIAL_HPP

#include <QObject>
#include <QtSerialPort>
#include <QQueue>
#include "SensorValue.hpp"
#include "Config.hpp"


class ImprovedSerial : public QObject
{
    Q_OBJECT
public:
    explicit ImprovedSerial(QSerialPort &port, QObject *parent = nullptr);

signals:
    void receivedConfig(Configuration conf);

    void receivedCommand(WeatherCommand command, quint8 argument);

    void receivedData(SensorValue val);

    void receivedDataPack(QVector<SensorValue> values);

public slots:
    void readData();


private:
    QSerialPort &m_port;

    bool m_started = false;

    quint16 toSize(quint8 byte1, quint8 byte2);

    QQueue<quint8> &waitNextBytes(QQueue<quint8> &stream, quint16 bytes);

    SensorValue readlSensorValue(QQueue<quint8> &stream);

    QVector<SensorValue> readPack(QQueue<quint8> &stream);

    Configuration readConfiguration(QQueue<quint8> &stream);

    void readCommand(QQueue<quint8> &stream, WeatherCommand command);

    static QQueue<quint8> enQueue(const QByteArray &arr, QQueue<quint8> &target);
};

#endif // IMPROVEDSERIAL_HPP

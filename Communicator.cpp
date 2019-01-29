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
 * File:   Communicator.cpp
 * Author: azarias
 *
 * Created on 15/12/2018
 */
#include "Communicator.hpp"
#include <QDebug>

Communicator::Communicator(QSerialPort &port, QObject *parent):
    QObject (parent),
    m_port(port)
{
    connect(&m_port, &QSerialPort::readyRead, this, &Communicator::readSerial);
}

bool Communicator::connectTo(const QString &serialPort)
{
    if(m_port.isOpen()){
        m_port.close();
    }

    m_port.setPortName(serialPort);
    return m_port.open(QIODevice::ReadWrite);
}

bool Communicator::sendCommand(WeatherCommand command, qint8 argument)
{
    if(!m_port.isOpen()) return false;

    const char data[] = {static_cast<qint8>(command), argument};

    bool written =  m_port.write(data, 2) == 2;
    return written && m_port.flush();
}

SensorValue Communicator::toSensorValue(char d1, char d2)
{
    SensorValue sv;
    sv.sensorId = (d1 & 0b00001100) >> 2;
    sv.value = static_cast<qint16>(static_cast<qint16>(d1) << 8 ) | (static_cast<qint16>(d2) & TEN_BITS);
    return sv;
}


void Communicator::readSerial()
{
    QByteArray data = m_port.readAll();
    if(data.size() == 0){
        qWarning() << "Empty data";
    }

    char first = data.at(0);
    qDebug() << "Received data of size " << data.size() << " code = " << static_cast<int>(first);

    char d1, d2;
    switch (first) {
    case 0x0:
        if(data.size() <3) {
            qWarning() << "Received invalid sensor data";
            return;
        }
        d1 = data.at(1);
        d2 = data.at(2);
        {
            SensorValue val = toSensorValue(d1, d2);
            qDebug() << "Received value from sensor " << val.sensorId << " = " << val.value;
        }
        return;
    default:// emit message or something for other cases
        return;
    }
}

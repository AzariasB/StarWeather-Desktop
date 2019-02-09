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
 * File:   ImprovedSerial.cpp
 * Author: azarias
 *
 * Created on 1/2/2019
 */
#include "ImprovedSerial.hpp"

ImprovedSerial::ImprovedSerial(QSerialPort &port, QObject *parent) :
    QObject(parent),
    m_port(port)
{
    connect(&m_port, &QSerialPort::readyRead, this, &ImprovedSerial::readData);
}

ImprovedSerial::~ImprovedSerial()
{
    m_port.close();
}

void ImprovedSerial::readData()
{
    // Disconnect, to let us read data when needed
    disconnect(&m_port, &QSerialPort::readyRead, this, &ImprovedSerial::readData);
    QQueue<quint8> stream;

    do{
        QByteArray read = m_port.readAll();
        enQueue(read, stream);

        WeatherCommand command = WeatherCommand(stream.dequeue());
        switch (command) {
        case SEND_MODE1_DATA:
            emit receivedData(readlSensorValue(stream));
            break;
        case SEND_MODE2_DATA:
        case GET_DATA:
            emit receivedDataPack(readPack(stream));
            break;
        case GET_FREQUENCIES:
            emit receivedConfig(readConfiguration(stream));
            break;
        case DEBUG:
            readDebug(stream);
            break;
        default:
            readCommand(stream, command);
            break;
        }
    } while(m_port.waitForReadyRead(0) || !stream.isEmpty());

    // Reconnect, to be warned when there is something to read
    connect(&m_port, &QSerialPort::readyRead, this, &ImprovedSerial::readData);
}

bool ImprovedSerial::sendCommand(WeatherCommand command, char argument)
{
    QByteArray data;
    data.append(command);
    if(argument != 0){
        data.append(argument);
    }
    return (m_port.write(data) == data.size()) && m_port.flush();
}

quint16 ImprovedSerial::toSize(quint8 byte1, quint8 byte2)
{
    return quint16( (quint16(byte1) << 8) | quint16(byte2));
}

WeatherCommand ImprovedSerial::currentMode() const
{
    return m_currentMode;
}

void ImprovedSerial::readDebug(QQueue<quint8> &stream)
{
    QByteArray total;
    while(true){
        waitNextBytes(stream, 1);
        char nw = stream.dequeue();
        if(nw == '~') break;
        total.append(nw);
    }
    qDebug() << "[DEBUG] " << QString(total);
}

void ImprovedSerial::readCommand(QQueue<quint8> &stream, WeatherCommand command)
{
    waitNextBytes(stream, 1);
    quint8 code = stream.dequeue();
    if(code == 0 && command <= WeatherCommand::START_MODE_3){
        m_currentMode = command;
        m_started = command != WeatherCommand::STOP_MODE;
    }
    emit receivedCommand(command, code);
}

bool ImprovedSerial::sendConfiguration(const Configuration &conf)
{
    bool all = true;
    all = sendCommand(WeatherCommand::CONFIGURE_FE_1, conf.freq1) && all;
    all = sendCommand(WeatherCommand::CONFIGURE_FE_2, conf.freq2) && all;
    all = sendCommand(WeatherCommand::CONFIGURE_FE_3, conf.freq3) && all;
    all = sendCommand(WeatherCommand::CONFIGURE_MODE_2, conf.mode2Time) && all;
    return all;

}

Configuration ImprovedSerial::readConfiguration(QQueue<quint8> &stream)
{
    waitNextBytes(stream, 4);
    Configuration conf;
    conf.freq1 = qMax(quint8(1), stream.dequeue());
    conf.freq2 = qMax(quint8(1), stream.dequeue());
    conf.freq3 = qMax(quint8(1), stream.dequeue());
    conf.mode2Time = stream.dequeue();
    return conf;
}

QVector<SensorValue> ImprovedSerial::readPack(QQueue<quint8> &stream)
{
    waitNextBytes(stream, 2);
    quint8 byte1 = stream.dequeue();
    quint8 byte2 = stream.dequeue();
    quint16 size = toSize(byte1, byte2);
    QVector<SensorValue> values;
    for(quint16 i = 0; i < size; ++i){
        values << readlSensorValue(stream);
    }
    return values;
}

SensorValue ImprovedSerial::readlSensorValue(QQueue<quint8> &stream)
{
    waitNextBytes(stream, TIMESTAMP_SIZE + 2);
    timestamp_t timestamp = 0;
    for(int i = 0; i < TIMESTAMP_SIZE; ++i){
        timestamp <<= 8;
        timestamp |= stream.dequeue();
    }
    quint8 byte1 = stream.dequeue();
    quint8 byte2 = stream.dequeue();
    return SensorValue(timestamp, byte1, byte2);
}

QQueue<quint8> &ImprovedSerial::waitNextBytes(QQueue<quint8> &stream, quint16 bytes)
{
    if(stream.size() >= bytes) return stream;

    while(stream.size() < bytes){
        while(!m_port.waitForReadyRead(-1));

        enQueue(m_port.readAll(), stream);
    }

    return stream;
}

QQueue<quint8> ImprovedSerial::enQueue(const QByteArray &arr, QQueue<quint8> &target)
{
    for(const quint8 byte : arr){
        target.enqueue(byte);
    }
    return target;
}

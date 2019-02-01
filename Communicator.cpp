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

    const char data[] = {qint8(command), argument};

    bool written =  m_port.write(data, 2) == 2;
    return written && m_port.flush();
}


QVector<SensorValue> Communicator::readPack(QQueue<char> &queue, quint16 size)
{
    QVector<SensorValue> values;
    while(queue.size() >= 2 && size > 0){
        char byte1 = queue.dequeue();
        char byte2 = queue.dequeue();
        values.append(SensorValue(byte1, byte2));
        size--;
    }

    return values;
}

quint16 Communicator::toSize(char byte1, char byte2)
{
    return quint16( (quint16(byte1) << 8) | quint16(byte2));
}

void Communicator::parseCommand(QQueue<char> &queue)
{
    char first = queue.dequeue();
    switch (first) {
        case SEND_MODE1_DATA:
        if(queue.size() < 2){
            qWarning() << "[MODE 1]Received incorrect data";
            return;
        }
        {
            char byte1 = queue.dequeue();
            char byte2 = queue.dequeue();
            emit receivedValue(SensorValue(byte1, byte2));
        }
        break;
    case SEND_MODE2_DATA:
    case GET_DATA:
        if(queue.size() < 2){
            qWarning() << "[MODE 2] Received incorrect data";
            return;
        }
        {
            char byte1 = queue.dequeue();
            char byte2 = queue.dequeue();
            quint16 size = toSize(byte1, byte2);
            if(queue.size() < (size * 2)){
                qWarning() << "[MODE 2] Received less data than expected :" << queue.size() << " vs " << (size*2);
                return;
            }
            emit receivedPack(readPack(queue, size));
        }
        break;
    default:
        char error = queue.dequeue();
        if(error){
            qWarning() << "Configuration failed";
        } else {
            emit confirmCommand(WeatherCommand(first), error);
        }
        break;
    }
}

void Communicator::readSerial()
{    
    // Using a queue because multiple data can arrive at the same time
    QByteArray data = m_port.readAll();
    if(data.size() == 0){
        qWarning() << "Empty data";
        return;
    }
    QQueue<char> queue;
    for(char c : data){
        queue.enqueue(c);
    }

    while(!queue.isEmpty()){
        parseCommand(queue);
    }

}

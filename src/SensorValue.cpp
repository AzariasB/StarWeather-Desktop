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
 * File:   SensorValue.cpp
 * Author: azarias
 *
 * Created on 31/1/2019
 */
#include "SensorValue.hpp"
#include <QtDebug>
#include <QString>

SensorValue::SensorValue(timestamp_t timestamp, qint8 byte1, qint8 byte2):
    m_timestamp(timestamp)
{
    quint16 combined = quint16(quint16(byte1) << 8);
    combined |=  qint16(byte2) & 0x00FF;
    m_value = combined & VALUE_MASK;
    m_sensorId = (combined & SENSORID_MASK) >> 10;
}


qint16 SensorValue::value() const
{
    return m_value;
}

qint8 SensorValue::sensorId() const
{
    return m_sensorId;
}

quint32 SensorValue::timestemp() const
{
    return m_timestamp;
}

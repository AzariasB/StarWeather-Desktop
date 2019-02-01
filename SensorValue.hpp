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
 * File:   SensorValue.hpp
 * Author: azarias
 *
 * Created on 31/1/2019
 */
#ifndef SENSORVALUE_HPP
#define SENSORVALUE_HPP

#include <qglobal.h>


constexpr quint16 VALUE_MASK     = 0b0000001111111111;
constexpr quint16 SENSORID_MASK  = 0b0000110000000000;


class SensorValue
{
public:
    SensorValue() = default;

    SensorValue(qint8 byte1, qint8 byte2);

    qint8 sensorId() const;

    qint16 value() const;

    quint32 timestemp() const;

    void setTimestamp(quint32 tst);

private:
    qint8 m_sensorId = 0;
    qint16 m_value = 0;
    quint32 m_timestamp = 0;
};

#endif // SENSORVALUE_HPP

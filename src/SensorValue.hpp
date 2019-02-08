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
#include "Config.hpp"

/**
 * @brief The SensorValue class
 * Contains all the data sent from the arduino
 * to represent the value sensed by a sensor
 */
class SensorValue
{
public:
    /**
      *
      * Empty constructor needed for the QVector and such
    */
    SensorValue() = default;

    /**
     * @brief SensorValue constructor
     * @param timestamp timestamp of the sensed value
     * @param byte1 first byte sent by the arduino
     * @param byte2 second byte sent by the arduino
     */
    SensorValue(timestamp_t timestamp, qint8 byte1, qint8 byte2);

    /**
     * @brief sensorId id of the sensor (between 1 and 3)
     * @return
     */
    qint8 sensorId() const;

    /**
     * @brief value value sensed by the sensor
     * @return
     */
    qint16 value() const;

    /**
     * @brief timestemp time when the value is sensed
     * @return
     */
    quint32 timestamp() const;

private:
    /**
     * @brief m_sensorId id of the sensor
     */
    qint8 m_sensorId = 0;

    /**
     * @brief m_value value sensed (is on ten bits, so 16 bits unsigned is needed)
     */
    qint16 m_value = 0;

    /**
     * @brief m_timestamp time when the value was sensed
     */
    timestamp_t m_timestamp = 0;
};

#endif // SENSORVALUE_HPP

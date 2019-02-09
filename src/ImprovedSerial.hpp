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

/**
 * @brief The ImprovedSerial class
 * used to communicate with the arduino,
 * sends commands, receives values and emit a signal
 * when a full packet is received
 */
class ImprovedSerial : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ImprovedSerial constructor
     * @param port serial port used to communicate with the arduino, must be initialized
     * @param parent parent object
     */
    explicit ImprovedSerial(QSerialPort &port, QObject *parent = nullptr);

    /**
     * @brief sendCommand sends a command to the arduino
     * @param command command to send
     * @param argument optional argument to send too
     * @return wether the data was sent (but does not say if
     * the command was successfully executed on the arduino)
     */
    bool sendCommand(WeatherCommand command, char argument = 0);

    /**
     * @brief currentMode
     * @return the current mode in the ardunio
     */
    WeatherCommand currentMode() const;

    /**
     * @brief sendConfiguration sends several packets to set the configuration on the arduino
     * @param conf configuration object to use to send to the arduino
     * @return if all the commands where sent
     */
    bool sendConfiguration(const Configuration &conf);

    ~ImprovedSerial();

signals:
    /**
     * @brief receivedConfig whenever the arduino sent its configuration
     * @param conf
     */
    void receivedConfig(Configuration conf);

    /**
     * @brief receivedCommand when a command has been received
     * @param command command received
     * @param argument argument received with it (may be 0)
     */
    void receivedCommand(WeatherCommand command, quint8 argument);

    /**
     * @brief receivedData when the arduino sends a value of a single sensor
     * @param val the value sensed on the arduino
     */
    void receivedData(SensorValue val);

    /**
     * @brief receivedDataPack when the arduino sends several values at once
     * @param values all the values sent by the arduino
     */
    void receivedDataPack(QVector<SensorValue> values);

public slots:
    /**
     * @brief readData when the arduino sends data for the first time
     * start the 'parsing' of the command to check what needs to be done
     */
    void readData();


private:
    /**
     * @brief m_port port used to communicate with the arduino
     */
    QSerialPort &m_port;

    /**
     * @brief m_started wether the arduino is running
     */
    bool m_started = false;

    /**
     * @brief m_currentMode current mode on the arduino
     */
    WeatherCommand m_currentMode = WeatherCommand::STOP_MODE;

    /**
     * @brief toSize concats the two bytes into one
     * @param byte1
     * @param byte2
     * @return
     */
    quint16 toSize(quint8 byte1, quint8 byte2);

    /**
     * @brief waitNextBytes interal method used to wait whever the packets arrive with a time interval
     * used to ensure that the queue has the indicated number of bytes
     * @param stream queue used as a stream to parse the command and the values
     * @param bytes minimum size the queue must be before continuing the execution flow
     * @return the queue when it has at least 'bytes' values
     */
    QQueue<quint8> &waitNextBytes(QQueue<quint8> &stream, quint16 bytes);

    /**
     * @brief readlSensorValue reads a single value sent by the sensor
     * @param stream queue containing the value of the sensor
     * @return the value that was read
     */
    SensorValue readlSensorValue(QQueue<quint8> &stream);

    /**
     * @brief readPack reads several values sent by the arduino
     * @param stream queue containing the values
     * @return a list of all the read values
     */
    QVector<SensorValue> readPack(QQueue<quint8> &stream);

    /**
     * @brief receivedDebug whenever the
     * arduino sends data for debugging purpose
     */
    void readDebug(QQueue<quint8> &stream);

    /**
     * @brief readConfiguration read the confgiuration sent by the arduino
     * @param stream queue conatining the data
     * @return the configuration read
     */
    Configuration readConfiguration(QQueue<quint8> &stream);

    /**
     * @brief readCommand when a command is sent, it may contain arguments
     * this read them and emit a signal
     * @param stream the queue that may contain the data
     * @param command command sent by the arduino
     */
    void readCommand(QQueue<quint8> &stream, WeatherCommand command);

    /**
     * @brief enQueue turns a byte array  into a queue
     * @param arr
     * @param target
     * @return
     */
    static QQueue<quint8> enQueue(const QByteArray &arr, QQueue<quint8> &target);
};

#endif // IMPROVEDSERIAL_HPP

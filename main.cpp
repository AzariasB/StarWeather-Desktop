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
 * File:   main.cpp
 * Author: azarias
 *
 * Created on 14/12/2018
 */
#include "MainWindow.hpp"
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QtSerialPort>
#include "Communicator.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSerialPort port("./virtual-tty");
    if(!port.open(QIODevice::ReadWrite)){
        qWarning() << port.errorString() << "\n" << port.error();
        return -1;
    }

    /*
     * const char send[] = {'a','b'};
    port.write(send, 2);

    if(!port.flush()){
        qWarning() << "No data written\n";
    } else {
        qDebug() << "Data written";
    }

    QTimer timer;
    timer.setSingleShot(true);

    QObject::connect(&port, &QSerialPort::readyRead, [&port, &timer](){
        qDebug() << QString(port.readAll());
        timer.start(0);
    });

    QObject::connect(&timer, &QTimer::timeout, [&port](){
        const char toSend[] = {'a','b'};
        port.write(toSend, 2);
        if(!port.flush()){
            qWarning() << "No data written\n";
        } else {
            qDebug() << "Data written";
        }
    });
    */

    Communicator c(port);
    if(c.sendCommand(WeatherCommand::START_MODE_2)) {
        qDebug() << "Starting mode";
    } else {
        qDebug() << "Failed to start mode";
    }

    QObject::connect(&c, &Communicator::receivedValue, [](SensorValue val){
        qDebug() << "Received value " << val.value << " from sensor " << val.sensorId << " at frequency " << val.frequency;
    });

    QObject::connect(&c, &Communicator::receivedPack, [](QVector<SensorValue> vals){
        qDebug() << "Received pack of " << vals.size() << " values";
        for(SensorValue val : vals){
            qDebug() << "   Value " << val.value << " from sensor " << val.sensorId << " at frequency " << val.frequency;
        }
    });

    return a.exec();
}

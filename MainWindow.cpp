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
 * File:   MainWindow.cpp
 * Author: azarias
 *
 * Created on 14/12/2018
 */
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "SensorValue.hpp"
#include <QtDebug>

MainWindow::MainWindow(Communicator &comm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_communicator(comm)
{
    ui->setupUi(this);

    connect(&m_communicator, &Communicator::receivedValue, [this](SensorValue val){
        val.setTimestamp(m_times[val.sensorId()-1].nextTime());
        ui->graph->drawSensorValue(val);
        ui->graph->redraw();
    });

    connect(&m_communicator, &Communicator::receivedPack, [this](QVector<SensorValue> vals){
        if(m_communicator.currentMode() == WeatherCommand::START_MODE_3){
            ui->mode3DataPushButton->setEnabled(true);
        }
        for(SensorValue val : vals){
            val.setTimestamp(m_times[val.sensorId()-1].nextTime());
            ui->graph->drawSensorValue(val);
        }
        ui->graph->redraw();
    });

    connect(ui->mode3DataPushButton, &QPushButton::clicked, [this](bool){
        ui->mode3DataPushButton->setEnabled(false);
        m_communicator.sendCommand(WeatherCommand::GET_DATA);
    });

    connect(&m_communicator, &Communicator::confirmCommand, this, &MainWindow::arduinoConfirm);
    connect(&m_communicator, &Communicator::receivedFrequencies, this, &MainWindow::setFrequencies);

    int id = 1;
    for(auto *btn : ui->modeGroup->buttons()){
        ui->modeGroup->setId(btn, id);
        ++id;
    }

    m_communicator.sendCommand(WeatherCommand::STOP_START_MODE);
}

void MainWindow::setMode()
{
    ui->groupBoxMode->setEnabled(false); // disable, will be enable when arduino answers back
    WeatherCommand command = static_cast<WeatherCommand>(ui->modeGroup->checkedId());
    m_communicator.sendCommand(command);
}

void MainWindow::setFrequencies(quint8 freq1, quint8 freq2, quint8 freq3, quint8 mode2Freq)
{
    qDebug() << "Freq 1 = " << freq1 << ", freq2 = " << freq2 << ", freq3 = " << freq3;
    freq1 = qMax(freq1, quint8(1));
    ui->sensor1Slider->setValue(freq1);
    freq2 = qMax(freq2, quint8(1));
    ui->sensor2Slider->setValue(freq2);
    freq3 = qMax(freq3, quint8(1));
    ui->sensor3Slider->setValue(freq3);
    ui->mode2Frequency->setValue(1000.f / mode2Freq);
    m_times = {
        SensorTime(freq1),
        SensorTime(freq2),
        SensorTime(freq3)
    };
}

void MainWindow::arduinoConfirm(WeatherCommand command, qint8 code)
{
    qDebug() << "Confirm " << command << " code = " << code;
    if(command <= 3){
        ui->groupBoxMode->setEnabled(true);
        ui->mode3DataPushButton->setEnabled(command == 3);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

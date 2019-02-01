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
        qDebug() << "Received value " << val.value() << " from sensor " << val.sensorId();
        ui->graph->drawSensorValue(val);
        ui->graph->redraw();
    });

    connect(&m_communicator, &Communicator::receivedPack, [this](QVector<SensorValue> vals){
        for(SensorValue val : vals){
            ui->graph->drawSensorValue(val);
        }
        ui->graph->redraw();
    });

    connect(&m_communicator, &Communicator::confirmCommand, this, &MainWindow::arduinoConfirm);

    int id = 1;
    for(auto *btn : ui->modeGroup->buttons()){
        ui->modeGroup->setId(btn, id);
        ++id;
    }
}

void MainWindow::setMode()
{
    ui->groupBoxMode->setEnabled(false); // disable, will be enable when arduino answers back
    WeatherCommand command = static_cast<WeatherCommand>(ui->modeGroup->checkedId());
    m_communicator.sendCommand(command);
}

void MainWindow::arduinoConfirm(WeatherCommand command, qint8 code)
{
    qDebug() << "Confirm " << command << " code = " << code;
    if(command <= 3){
        ui->groupBoxMode->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

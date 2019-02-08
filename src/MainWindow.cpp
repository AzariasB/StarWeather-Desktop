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
#include <QMessageBox>
#include <QLineSeries>
#include <QChartView>

MainWindow::MainWindow(ImprovedSerial &comm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_communicator(comm)
{
    ui->setupUi(this);
    ui->chartView->setChart(m_chart = new SensorChart());

    connect(&m_communicator, &ImprovedSerial::receivedData, [this](SensorValue val){
        m_chart->drawSensorValue(val);
    });

    connect(&m_communicator, &ImprovedSerial::receivedDataPack, [this](QVector<SensorValue> vals){
        if(m_communicator.currentMode() == WeatherCommand::START_MODE_3){
            ui->mode3DataPushButton->setEnabled(true);
        }
        for(SensorValue val : vals){
            m_chart->drawSensorValue(val);
        }
    });

    connect(&m_communicator, &ImprovedSerial::receivedCommand, this, &MainWindow::arduinoConfirm);
    connect(&m_communicator, &ImprovedSerial::receivedConfig, this, &MainWindow::setFrequencies);

    int id = 0;
    for(auto *btn : ui->modeGroup->buttons()){
        ui->modeGroup->setId(btn, id);
        ++id;
    }

    m_communicator.sendCommand(WeatherCommand::GET_FREQUENCIES);
}

void MainWindow::getMode3Data()
{
    ui->mode3DataPushButton->setEnabled(false);
    m_communicator.sendCommand(WeatherCommand::GET_DATA);
}

void MainWindow::setMode()
{
    ui->groupBoxMode->setEnabled(false); // disable, will be enable when arduino answers back
    WeatherCommand command = static_cast<WeatherCommand>(ui->modeGroup->checkedId());
    m_communicator.sendCommand(command);
}

void MainWindow::sendFrequencies()
{
    Configuration conf;
    conf.freq1 = ui->sensor1Slider->value();
    conf.freq2 = ui->sensor2Slider->value();
    conf.freq3 = ui->sensor3Slider->value();
    conf.mode2Time = ui->mode2Frequency->value();
    m_communicator.sendConfiguration(conf);

    ui->sensor1Slider->setEnabled(false);
    ui->sensor2Slider->setEnabled(false);
    ui->sensor3Slider->setEnabled(false);
    ui->mode2Frequency->setEnabled(false);
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, "A propos de Qt");
}

void MainWindow::about()
{
    QMessageBox::about(this, "A propos", "Station météo - version PC\n Programmé par Azarias Boutin");
}

void MainWindow::setFrequencies(Configuration conf)
{
    ui->sensor1Slider->setValue(conf.freq1);
    ui->sensor2Slider->setValue(conf.freq2);
    ui->sensor3Slider->setValue(conf.freq3);
    ui->mode2Frequency->setValue(conf.mode2Time);
}

void MainWindow::arduinoConfirm(WeatherCommand command, qint8 code)
{
    if(command <= 3){
        ui->groupBoxMode->setEnabled(true);
        ui->mode3DataPushButton->setEnabled(command == 3);
        return;
    }
    switch (command) {
    case WeatherCommand::CONFIGURE_FE_1:
        ui->sensor1Slider->setEnabled(true);
        return;
    case WeatherCommand::CONFIGURE_FE_2:
        ui->sensor2Slider->setEnabled(true);
        return;
    case WeatherCommand::CONFIGURE_FE_3:
        ui->sensor3Slider->setEnabled(true);
        return;
    case WeatherCommand::CONFIGURE_MODE_2:
        ui->mode2Frequency->setEnabled(true);
        return;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

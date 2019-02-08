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
 * File:   SensorChart.cpp
 * Author: azarias
 *
 * Created on 7/2/2019
 */
#include "SensorChart.hpp"
#include "SensorValue.hpp"
#include <QtDebug>
#include <QValueAxis>

SensorChart::SensorChart() : QtCharts::QChart ()
{
    m_xAxis = new QtCharts::QValueAxis();
    auto *yAxis = new QtCharts::QValueAxis();
    yAxis->setRange(0, 1024);
    m_xAxis->setRange(0, 1000);
    m_xAxis->setTickCount(5);
    m_xAxis->setTitleText("Temps (s)");
    yAxis->setTitleText("Valeur");
    addAxis(m_xAxis, Qt::AlignBottom);
    addAxis(yAxis, Qt::AlignLeft);

    for(int i = 0; i < 3; ++i){
        auto serie = new QtCharts::QSplineSeries();
        serie->setPointsVisible(true);
        addSeries(serie);
        serie->attachAxis(m_xAxis);
        serie->attachAxis(yAxis);
        serie->setName("Détecteur " + QString::number(i+1));
    }


    legend()->show();
    setTitle("Valeurs arduino");
    setAnimationOptions(QtCharts::QChart::AllAnimations);
}

qreal SensorChart::toSeconds(quint32 milliseconds)
{
    return qreal(milliseconds / 10000.0);
}

void SensorChart::redraw()
{

}

void SensorChart::drawSensorValue(const SensorValue &value)
{
    qreal seconds = toSeconds(value.timestamp());
    static_cast<QtCharts::QSplineSeries*>(series().takeAt(value.sensorId()-1))->append(seconds, value.value());
    if(seconds > m_xAxis->max()){
        scroll(plotArea().width() / 2.0, 0);
    }
}

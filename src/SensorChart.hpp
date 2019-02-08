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
 * File:   SensorChart.hpp
 * Author: azarias
 *
 * Created on 7/2/2019
 */
#ifndef SENSORCHART_HPP
#define SENSORCHART_HPP

#include <QWidget>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>

class SensorValue;

class SensorChart : public QtCharts::QChart
{
    Q_OBJECT
public:
    explicit SensorChart();

    void drawSensorValue(const SensorValue &value);

    void redraw();

private:
    qreal m_maxT = 50;

    qreal toSeconds(quint32 milliseconds);

    QtCharts::QValueAxis *m_xAxis;
};

#endif // SENSORCHART_HPP
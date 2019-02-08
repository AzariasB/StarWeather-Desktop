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
 * File:   MainWindow.hpp
 * Author: azarias
 *
 * Created on 14/12/2018
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTime>
#include <QLineSeries>
#include "SensorChart.hpp"
#include "ImprovedSerial.hpp"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ImprovedSerial &comm, QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    /**
     * @brief setMode changes the mode on the arduino
     */
    void setMode();

    /**
     * @brief arduinoConfirm when the arduino confirms that a certain command has
     * been completed (or not)
     * @param command the command executed
     * @param code error code : 0 means success, otherwiset, means error
     */
    void arduinoConfirm(WeatherCommand command, qint8 code);

    /**
     * @brief setFrequencies whenever the aruino sends its configuration,
     * updates the UI
     * @param conf
     */
    void setFrequencies(Configuration conf);

    /**
     * @brief sendFrequencies send the conf chosen by the user
     * to the arduino
     */
    void sendFrequencies();

    /**
     * @brief getMode3Data when the user clicks the button 'get mode 3 data'
     * sends the command to the arduino to get the data
     */
    void getMode3Data();

    /**
     * @brief aboutQt shows the dialog 'about qt'
     */
    void aboutQt();

    /**
     * @brief about shows the dialog 'about'
     */
    void about();

private:
    Ui::MainWindow *ui;

    /**
     * @brief m_communicator used to communicate with the ardunio
     */
    ImprovedSerial &m_communicator;

    SensorChart *m_chart;
};

#endif // MAINWINDOW_HPP

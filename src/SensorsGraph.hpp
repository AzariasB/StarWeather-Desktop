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
 * File:   SensorsGraph.hpp
 * Author: azarias
 *
 * Created on 31/1/2019
 */
#ifndef SENSORSGRAPH_HPP
#define SENSORSGRAPH_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include "SensorValue.hpp"

constexpr int MAX_Y = 1024;
constexpr int X_STEP = 50;
constexpr int X_VALUES = 200;
constexpr int Y_VALUES = 6;

/**
 * @brief The SensorsGraph class
 * used to draw the data received from the arduino
 * features a simple transition to avoid the graph
 * going too much on the right and becoming invisible
 * supports resizing
 * Is optimized to render any amount of data
 * by deleting the data that is not represented
 * when it is too far on the left
 */
class SensorsGraph : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(qreal xTranslate READ xTranslate WRITE setXTranslate)
public:
    /**
     * @brief SensorsGraph constructor
     * @param parent widget parent
     */
    SensorsGraph(QWidget *parent);

    /**
     * @brief drawSensorValue adds the given value to the sensor list to draw
     * does not update the graph yet, 'redraw' must be called to do so
     * @param value the value to draw
     */
    void drawSensorValue(const SensorValue &value);

    /**
     * @brief redraw redraws ALL the element on the graphic scene
     * this function is costly and must not be called too frequently
     */
    void redraw();

    /**
     * @brief setXTranslate setter for the property, used for the animation
     * @param nwValue
     */
    void setXTranslate(qreal nwValue);

    /**
     * @brief xTranslate getter for the xTranslate value
     * @return
     */
    qreal xTranslate() const;

protected:
    /**
     * @brief showEvent inherited method
     * @param ev
     */
    void showEvent(QShowEvent *ev) override;

    /**
     * @brief resizeEvent supporting the window resizing
     * redraws the graph with the new size
     * @param ev
     */
    void resizeEvent(QResizeEvent *ev) override;

private:

    /**
     * @brief m_scene scene where all the elements are added
     * to be drawn
     */
    QGraphicsScene m_scene;

    /**
     * @brief m_xTranslate translation on the x axis
     * is used to create a smooth transition
     */
    qreal m_xTranslate = 0;

    /**
     * @brief m_animation object used to animate the graph
     */
    QPropertyAnimation m_animation = QPropertyAnimation(this, "xTranslate");

    /**
     * @brief m_values all the values drawn on the graph
     * The root vector contains 3 vector, one for each sensor
     * then the child vector contains all the value for the given sensor
     * This is used to optmize the removing time whenever a value is too much
     * on the left
     */
    QVector<QVector<SensorValue>> m_values = QVector<QVector<SensorValue>>(3);

    /**
     * @brief clearOldValues removes the value not rendered on screen because
     * too much on the right
     * @param values all the values for a given sensor
     * @param polygon the polygon associated with the values (Vector of points)
     * @param left the leftMost part of the graph
     */
    void clearOldValues(QVector<SensorValue> &values, QPolygonF &polygon, qreal left);

    /**
     * @brief fillScene draws the main elements of the scene
     * (axis and legends) then draws the values
     */
    void fillScene();

    /**
     * @brief drawValues draws all the saved values, within the given viewport
     * @param viewport size of the view the graphs is drawn in
     * @param xStep number of pixels equals to a step between two X_STEP
     */
    void drawValues(const QRectF& viewport, qreal xStep);

    /**
     * @brief m_pens style pen used for the graphic
     */
    static QVector<QPen> m_pens;
};

#endif // SENSORSGRAPH_HPP

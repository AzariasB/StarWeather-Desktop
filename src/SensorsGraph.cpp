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
 * File:   SensorsGraph.cpp
 * Author: azarias
 *
 * Created on 31/1/2019
 */
#include "SensorsGraph.hpp"
#include <QtDebug>
#include <QShowEvent>
#include <QResizeEvent>
#include <QGraphicsTextItem>
#include <QElapsedTimer>

QVector<QPen> SensorsGraph::m_pens = {
    QPen(Qt::red, 3),
    QPen(Qt::green, 3),
    QPen(Qt::blue, 3)
};

SensorsGraph::SensorsGraph(QWidget *parent):
    QGraphicsView (parent)
{
    setScene(&m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    m_animation.setEasingCurve(QEasingCurve::InOutExpo);
}


void SensorsGraph::showEvent(QShowEvent *ev)
{
    QGraphicsView::showEvent(ev);
    fillScene();
}


void SensorsGraph::resizeEvent(QResizeEvent *ev)
{
    QGraphicsView::resizeEvent(ev);
    fillScene();
}

int toPointNumber(qreal size)
{
    return int(size / 15);
}

void SensorsGraph::drawSensorValue(const SensorValue &value)
{
    m_values[value.sensorId() - 1] << value;
}

void SensorsGraph::clearOldValues(QVector<SensorValue> &values, QPolygonF &polygon, qreal left)
{
    if(polygon.isEmpty() || polygon.first().x() > left) return;
    int toEraseIdx = 0;
    for(const auto &point: polygon){
        if(point.x() < left) toEraseIdx++;
        else break;
    }
    if(toEraseIdx > 1){
        values.remove(0, toEraseIdx - 1);
        polygon.remove(0, toEraseIdx - 1);
    }
}

void SensorsGraph::setXTranslate(qreal nwValue)
{
    m_xTranslate = nwValue;
    redraw();
}

qreal SensorsGraph::xTranslate() const
{
    return m_xTranslate;
}

void SensorsGraph::drawValues(const QRectF &viewport, qreal xStep)
{
    QVector<QPolygonF> allPoint(3);
    qreal mostRight = 0;
    for(int i = 0; i < 3; ++i){
        QPolygonF &pol = allPoint[i];
        const QVector<SensorValue> &values = m_values[i];
        for(const SensorValue &val: values){
            qreal yPosition = viewport.bottom() - (viewport.height() / MAX_Y) * val.value();
            qreal xPosition = (val.timestemp() / 1000.0) * (xStep / X_STEP);
            pol << QPointF(xPosition, yPosition);
            mostRight = qMax(xPosition, mostRight);
        }
    }

    // Translate all the points if needed
    if(mostRight > viewport.right()){
        qreal translate = (viewport.right() - mostRight) - (viewport.right() / 3);

        if(m_animation.state() != QAbstractAnimation::State::Running){
            m_animation.setDuration(1000);
            m_animation.setStartValue(xTranslate());
            m_animation.setEndValue(translate);
            m_animation.start();
        }
        for(int i = 0; i < 3; ++i){
            QPen &pen = m_pens[i];
            QPolygonF &points = allPoint[i];
            points.translate(xTranslate(), 0);
            clearOldValues(m_values[i], points, viewport.left());
            QPainterPath path;
            path.addPolygon(points);
            m_scene.addPath(path, pen);
        }
    } else {
        // Just draw the points
        for(int i = 0; i < 3; ++i){
            QPen &pen = m_pens[i];
            QPolygonF &points = allPoint[i];
            QPainterPath path;
            path.addPolygon(points);
            m_scene.addPath(path, pen);
        }
    }

    // Remove useless sensorValues (at the left of the graph)
}

void SensorsGraph::redraw()
{
    fillScene();
}

void SensorsGraph::fillScene()
{
    m_scene.clear();
    int top = 25;
    int left = 5;
    qreal right = width() - 10;
    qreal bottom = height() - 10;

    m_scene.addLine(5, top, 5, bottom + 5);
    m_scene.addLine(1, top, 9, top);
    m_scene.addLine(right - 5, bottom - 5, right, bottom);
    m_scene.addLine(right - 5, bottom + 5, right, bottom);

    m_scene.addLine( 0, bottom, right, bottom);
    QGraphicsTextItem *text = m_scene.addText("ms");
    text->setPos(right - text->boundingRect().width(), bottom - text->boundingRect().height());

    m_scene.addText("°");

    text = m_scene.addText(QString::number(MAX_Y));
    text->setPos(10, 10);

    // draw lines on x axis
    int points = toPointNumber(right - left);
    text = m_scene.addText(QString::number(X_STEP));
    qreal pixelStep = (right - left) / points;
    text->setPos(pixelStep, bottom - text->boundingRect().height());
    for(int i = 1; i < points; ++i){
        m_scene.addLine(i * pixelStep + left, bottom - 5, left + i*pixelStep, bottom + 5);
    }

    // draw lines on y axis
    qreal yPixelStep = (bottom - top) / Y_VALUES;
    int yStep = MAX_Y / Y_VALUES;
    for(int i = 1; i < Y_VALUES; ++i){
        m_scene.addLine( left - 5, top + i * yPixelStep, right, top + i * yPixelStep);
        text = m_scene.addText(QString::number((Y_VALUES  - i) * yStep));
        text->setPos(left, yPixelStep * i);
    }

    m_scene.setSceneRect(m_scene.itemsBoundingRect());
    drawValues(QRectF(left, top, right - left, bottom - top), pixelStep);
}

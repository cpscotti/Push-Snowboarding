/*
 * Copyright (c) 2011 Nokia Corporation
 *
 * This file is part of the Push Snowboarding Project, More info at:
 * www.pushsnowboading.com
 *
 * Author: Clovis Scotti <scotti@ieee.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef GRAPHICVARPLOT_H
#define GRAPHICVARPLOT_H

#include <QGraphicsObject>
#include <QPainter>
#include <QPainterPath>

#include <QGraphicsSceneMouseEvent>

#include <QPen>


#include <QObject>
#include <QColor>

#include <QList>
#include <QVector>
#include <QVariant>

#include <QDebug>

#include "math.h"

class GraphicVarPlot : public QGraphicsObject
{
    Q_OBJECT
//    Q_ENUMS(PlotType)
public:
    GraphicVarPlot(QGraphicsItem*, int);
    ~GraphicVarPlot();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

    void setColor(QColor clr);
    void setPlotTitle(QString name);

    void setVerticalRange(float min, float max);
    void setHorizontalRange(double min, double max);

    enum PlotType { Lines, Bars };
    void setPlotType(PlotType ptype);

    void setGraphPoints(QList<double> * points);
    void setGraphPoints(QList<double> * abscissa, QList<double> * points);

    int plotIdx;

    void setValTransform(double val);

    void addStatBox(const QString& value, const QString& name);
    void clearStatBoxes();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void hor_rel_slide(qreal dif);

signals:
    void hor_rel_drag(qreal ydrag);
    void swipeLeft();
    void swipeRigth();
    void bounceBack();

private:

    QPoint firstTch;
    QPoint prevTch;

    void generate_graph_path();
    void generate_bar_graph_path();

    void drawGrid(QPainter *painter);
    void drawGraph(QPainter *painter);

    void drawStatBoxes(QPainter *painter);

    QColor themeColor;
    QColor titleColor;
    QString plotTitle;
    QPen gridPen;
    QPen lowerGridPen;
    QPen graphPen;

    PlotType plotType;

    float vertMin;
    float vertMax;
    float vertGrid;
    double variableValueTransform;
    float vertTransform;

    double horMin;
    double horMax;
    float horTransform;

    QPainterPath * path;
    QPainterPath * grid;

    QList<double> * abscissaPoints;
    QList<double> * graphPoints;
    double invalidOrdValue;

    QVector<QString> statVal;
    QVector<QString> statName;

    static const float gHei = 200.0;
    static const float gWid = 338.0;
    static const float gWidGraphOffset = 4.0;
    static const float gYOffset = 30.0;
    static const int gCells = 7;

};

#endif // GRAPHICVARPLOT_H

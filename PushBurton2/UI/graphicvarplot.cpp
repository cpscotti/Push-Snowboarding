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

#include "graphicvarplot.h"

GraphicVarPlot::GraphicVarPlot(QGraphicsItem* parent=0, int idx = -1)
    : QGraphicsObject(parent), plotIdx(idx)

{
    this->setZValue(0.4);
    this->update();

    titleColor = QColor(17,83,161);

    gridPen.setWidthF(0.5);
    gridPen.setColor(Qt::gray);

    lowerGridPen.setWidthF(1.6);
    lowerGridPen.setColor(Qt::gray);

    graphPen.setWidthF(2.0);
    graphPen.setColor(themeColor);

    vertGrid = 10.0;
    horTransform = 1.0;

    graphPoints = 0;
    abscissaPoints = 0;
    path = 0;

    plotType = Lines;

    invalidOrdValue = -1.0;
    variableValueTransform = 1.0;
}

GraphicVarPlot::~GraphicVarPlot()
{
    if(path)
        delete path;
}

QRectF GraphicVarPlot::boundingRect() const
{
    return QRectF(QPointF(0.0,0.0), QSizeF(340,330));
}

void GraphicVarPlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
//    painter->setBrush(Qt::white);
//    painter->drawRect(QRect(0,0,340,254));

    painter->setPen(titleColor);
    painter->setFont(QFont("Nokia Standard Bold", 5));
    painter->drawText(4,18, plotTitle);

    drawGrid(painter);
    drawGraph(painter);
    drawStatBoxes(painter);
}

void GraphicVarPlot::drawGrid(QPainter *painter)
{
    for(int i=0;i<gCells;i++)
    {
        painter->setPen(gridPen);
        painter->drawLine(QPointF(2,(gHei/gCells)*i+gYOffset), QPointF(gWid,(gHei/gCells)*i+gYOffset));
        painter->setPen(titleColor);
        painter->setFont(QFont("Nokia Standard Bold", 4));
        painter->drawText(2,(gHei/gCells)*i+44, QString::number(((gCells-i)*vertGrid+vertMin)*variableValueTransform, 'f', 1));
    }

    painter->setPen(lowerGridPen);
    painter->drawLine(QPointF(2,(gHei/gCells)*gCells+gYOffset), QPointF(gWid,(gHei/gCells)*gCells+gYOffset));
}

void GraphicVarPlot::drawGraph(QPainter *painter)
{
    if(path)
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(graphPen);
        painter->drawPath(*path);
    }
}

void GraphicVarPlot::setColor(QColor clr)
{
    themeColor = clr;
    graphPen.setColor(themeColor);
}

void GraphicVarPlot::setPlotTitle(QString name)
{
    plotTitle = name;
}

void GraphicVarPlot::setVerticalRange(float min, float max)
{
    vertMax = max;
    vertMin = min;
    vertGrid = (max-min)/gCells;
    vertTransform = gHei/(max-min);
}

void GraphicVarPlot::setHorizontalRange(double min, double max)
{
    horMax = max;
    horMin = min;
    horTransform = gWid/(float)(max-min);
}

void GraphicVarPlot::setPlotType(PlotType ptype)
{
    plotType = ptype;
}

void GraphicVarPlot::setGraphPoints(QList<double> * points)
{
    if(plotType != Lines)
        return;
    if(points->count() > 1)
    {
        graphPoints = points;

//        qDebug() << "Max = " << vertMax << ", vertMin, " << vertMin  << "vertTransform = " << vertTransform;
        generate_graph_path();
    } else {
        delete path;
        path = 0;
        qDebug() << "Input graph has less than 2 points, impossible to walk on this mud";
    }
    this->update();
}

void GraphicVarPlot::setGraphPoints(QList<double> * abscissa, QList<double> * points)
{
    if(plotType != Bars)
        return;
    if(points->count() > 0 && points->count() == abscissa->count())
    {
        graphPoints = points;
        abscissaPoints = abscissa;

//        qDebug() << "vertMax = " << vertMax << ", vertMin, " << vertMin  << "vertTransform = " << vertTransform;
//        qDebug() << "horMax = " << QString::number(horMax,'f',3) << ", horMin, " << QString::number(horMin,'f',3) << "horTransform = " << horTransform;
        generate_bar_graph_path();

    } else {
        delete path;
        path = 0;
        qDebug() << "Input lists empty or corrupted, impossible to walk on this mud";
    }
    this->update();
}

void GraphicVarPlot::generate_graph_path()
{
    if(path)
        delete path;

    horTransform = (gWid-2*gWidGraphOffset)/graphPoints->count();

    QList<double>::iterator it;

    int i = 0;
    it = graphPoints->begin();
    path = new QPainterPath(
            QPointF(gWidGraphOffset,
                    (vertMax-(*it))*vertTransform+gYOffset));

    ++it;
    i++;
    while(it != graphPoints->end())
    {
        if(*it != invalidOrdValue)
            path->lineTo(gWidGraphOffset+i*horTransform, (vertMax-(*it))*vertTransform+gYOffset);
        ++it;
        i++;
    }
}

void GraphicVarPlot::generate_bar_graph_path()
{
    if(path)
        delete path;

    QList<double>::iterator vit;
    QList<double>::iterator ait;

    vit = graphPoints->begin();
    ait = abscissaPoints->begin();

    path = new QPainterPath(
            QPointF(gWidGraphOffset+(  (*ait - horMin)*horTransform /*transformed x(0)*/),
                    vertMax*vertTransform+gYOffset /*transformed y(0)*/));

    path->lineTo(gWidGraphOffset+(  (*ait - horMin)*horTransform  /*transformed x(0)*/),
                 (vertMax-(*vit))*vertTransform+gYOffset/*transformed y(0)*/);

    ++vit;
    ++ait;

    while(vit != graphPoints->end() && ait != abscissaPoints->end())
    {
        path->moveTo(gWidGraphOffset+(  ((float)(*ait - horMin))*horTransform ),
                     vertMax*vertTransform+gYOffset);
        path->lineTo(gWidGraphOffset+(  ((float)(*ait - horMin))*horTransform ),
                     (vertMax-(*vit))*vertTransform+gYOffset);

        ++vit;
        ++ait;
    }

}

void GraphicVarPlot::setValTransform(double val)
{
    variableValueTransform = val;
}

void GraphicVarPlot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    firstTch = event->screenPos();
    prevTch = event->screenPos();
}

void GraphicVarPlot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint now = event->screenPos();
    qreal flightDif = now.x() - firstTch.x();
    if(fabs(flightDif) > 340.0/4.0 ) {
        if(flightDif > 0) {
            emit swipeRigth();
        } else {
            emit swipeLeft();
        }
    } else {
        emit bounceBack();
    }
}

void GraphicVarPlot::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint now = event->screenPos();
    qreal relFlightDif = now.x() - prevTch.x();
    emit hor_rel_drag(relFlightDif);
    prevTch = now;
}

void GraphicVarPlot::hor_rel_slide(qreal dif)
{
    QVariant oldX = this->property("x");
    this->setProperty("x", oldX.toFloat()+dif);
}

void GraphicVarPlot::addStatBox(const QString& value, const QString& name)
{
    statVal.push_back(value);
    statName.push_back(name);
}

void GraphicVarPlot::clearStatBoxes()
{
    statVal.clear();
    statName.clear();
}

void GraphicVarPlot::drawStatBoxes(QPainter *painter)
{

    int n = statVal.count();
    if(n == 0)
        return;
    static const int margin = 2;
    static const int sep = 6;
    float boxWid = (gWid-margin-(n-1)*sep)/n;
//    qDebug() << "n = " << n << ", wid = " << boxWid;

    for(int i=0;i<n;i++) {

        painter->setBrush(themeColor);
        painter->setPen(Qt::NoPen);

        float bPosX = margin+i*(boxWid+sep);
//        qDebug() << "bPosX = " << bPosX << "boxWid = " << boxWid;

        QRect box = QRect(bPosX,gYOffset+gHei+14,boxWid,66);
        painter->drawRect(box);

        painter->setPen(Qt::white);

        painter->setFont(QFont("Nokia Standard Bold", 16));
        painter->drawText(box, Qt::AlignHCenter | Qt::AlignBottom, statVal[i]);

        painter->setFont(QFont("Nokia Standard Bold", 6));
        painter->drawText(box, Qt::AlignHCenter | Qt::AlignTop, statName[i]);
    }

}

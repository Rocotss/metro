#include "edge.h"
#include "station.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>

Station::Station(GraphWidget *graphWidget)
    :graph(graphWidget), cost(0)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setZValue(-1);
}

QRectF Station::boundingRect() const
{
    return QRectF(-20, -20, 20, 20);
}

QPainterPath Station::shape() const
{
    QPainterPath path;
    path.addEllipse(-20, -20, 20, 20);
    return path;
}

void Station::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(cost == 0)
    {
        if(!isSelected())
        {
            painter->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            painter->setBrush(Qt::red);
            painter->drawEllipse(-20, -20, 20, 20);
        }
        else
        {
            painter->setPen(QPen(Qt::blue, 3, Qt::DashLine));
            painter->setBrush(Qt::red);
            painter->drawEllipse(-20, -20, 20, 20);
        }
    }
    else
    {
        if(!isSelected())
        {
            painter->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            painter->setBrush(Qt::green);
            painter->drawEllipse(-20, -20, 20, 20);
        }
        else
        {
            painter->setPen(QPen(Qt::blue, 3, Qt::DashLine));
            painter->setBrush(Qt::green);
            painter->drawEllipse(-20, -20, 20, 20);
        }
    }

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
    painter->drawRect(boundingRect());

}

int Station::type() const
{

    return QGraphicsItem::UserType+2;
}

void Station::setCost(double input_cost)
{
    cost = input_cost;
}

unsigned int Station::getCost() const
{
    return cost;
}

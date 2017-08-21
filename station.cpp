#include "edge.h"
#include "station.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>

Station::Station(GraphWidget *graphWidget)
    :graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setSelected(true);
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
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->setBrush(Qt::red);
    painter->drawEllipse(-20, -20, 20, 20);
}

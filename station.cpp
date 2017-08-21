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
    setZValue(-1);
}

QRectF Station::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Station::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Station::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->setBrush(Qt::red);
    painter->drawEllipse(-10, -10, 20, 20);
}

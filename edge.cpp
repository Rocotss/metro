#include "edge.h"
#include "station.h"

#include <QPainter>

Edge::Edge(Station *sourceStation, Station *destStation)
{
    setZValue(0);
    source = sourceStation;
    dest = destStation;
    adjust();
}

Station *Edge::sourceStation() const
{
    return source;
}

Station *Edge::destStation() const
{
    return dest;
}

void Edge::adjust()
{
    if(!source || !dest)
        return;

    QLineF line( mapFromItem(source, 10, 10), mapFromItem(dest, 10, 10));
    qreal length = line.length();

    prepareGeometryChange();

    if(length > qreal(0.))
    {
        qreal penWidth = 15;
        QPointF edgeOffset(penWidth / 2, penWidth / 2);
        sourcePoint = source->pos() + edgeOffset;
        destPoint = dest->pos() - edgeOffset;
    }
    else
    {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if(!source || !dest)
    {
        return QRectF();
    }

    return QRectF(QPointF(source->pos().x(), source->pos().y()), QSizeF(dest->pos().x() - source->pos().x(),
                                        dest->pos().y() - source->pos().y()))
            .normalized()
            .adjusted(-20, -20, 20, 20);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(!source || !dest)
        return;

    QLineF line(source->pos() - QPointF(10, 10), dest->pos() - QPointF(10, 10));
    if(qFuzzyCompare(line.length(), qreal(-10.)))
        return;

    if(!isSelected())
    {
        painter->setPen(QPen(Qt::darkRed,15,Qt::SolidLine));
    }
    else
    {
        painter->setPen(QPen(Qt::darkBlue, 15, Qt::DotLine));
    }
    painter->drawLine(line);
}

int Edge::type() const
{
    return QGraphicsItem::UserType+1;
}


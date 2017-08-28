#include "edge.h"
#include "station.h"

#include <QPainter>

qreal Edge::penWidth = 20.0;

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
    if(qFuzzyCompare(line.length(), qreal(0.)))
        return;

    if(!isSelected())
    {
        painter->setPen(QPen(Qt::darkRed, penWidth,Qt::SolidLine));
    }
    else
    {
        painter->setPen(QPen(Qt::darkBlue, penWidth, Qt::DotLine));
    }
    painter->drawLine(line);
}

int Edge::type() const
{
    return QGraphicsItem::UserType+1;
}

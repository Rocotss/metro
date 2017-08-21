#include "edge.h"
#include "station.h"

#include <QPainter>

Edge::Edge(Station *sourceStation, Station *destStation)
{
    setFlag(ItemIsSelectable);
    setAcceptedMouseButtons(0);
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

    QLineF line( mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    if(length > qreal(20.))
    {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
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

    qreal penWidth = 10;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-penWidth, -penWidth, penWidth, penWidth);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if(qFuzzyCompare(line.length(), qreal(0.)))
        return;

    painter->setPen(QPen(Qt::red,10,Qt::SolidLine));
    painter->drawLine(line);
}

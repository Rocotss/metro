#include "edge.h"
#include "station.h"

#include <math.h>
#include <QPainter>

const qreal Edge::penWidth = 20.0;

Edge::Edge(Station *sourceStation, Station *destStation)
{
    setZValue(-0.5);
    source = sourceStation;
    dest = destStation;
}

QRectF Edge::boundingRect() const
{
    return QRectF(QPointF(source->pos().x(), source->pos().y()), QSizeF(dest->pos().x() - source->pos().x(),
                                        dest->pos().y() - source->pos().y()))
            .normalized()
            .adjusted(-20, -20, 0, 0);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
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

    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
    painter->drawLine(line);

    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
    painter->drawRect(boundingRect());
}

int Edge::type() const
{
    return QGraphicsItem::UserType+1;
}

qreal Edge::Cost(int type)
{
    enum {Shortest, Cheapest, Transfer};
    switch(type)
    {
        case Shortest: // поиск самого короткого пути. Считаем цену за прогон из его длины
            {
                qreal lengthLine = sqrt(pow(source->pos().x() - dest->pos().x() ,2) + pow(source->pos().y() - dest->pos().y(), 2));
                return dest->getCost() + lengthLine;
            }
        case Cheapest: // поиск самого дешёвого маршрута. Считаем цену станции плюс 1 за прогон
            {
                return dest->getCost() + 1;
            }
        case Transfer: // поиск маршрута с наименьшим количеством пересадак. Считаем пересадочные станции
            {
                if(dest->getCost()>0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
    }
}

Station getSource()
{
    return source;
}

Station getDest()
{
    return dest;
}

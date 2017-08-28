#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Station;

class Edge : public QGraphicsItem
{
public:
    Edge(Station *sourceStation, Station *destStation);

    Station *sourceStation() const;
    Station *destStation() const;

    int type() const override;

    void adjust();

    static qreal penWidth;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

private:
    Station *source;
    Station *dest;
    QPointF sourcePoint;
    QPointF destPoint;
};

#endif // EDGE_H

#ifndef STATION_H
#define STATION_H

#include <QGraphicsItem>
#include <string>
#include "string.h"

class GraphWidget;

class Station : public QGraphicsItem
{
public:
    Station(GraphWidget *graphWidget);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

    int type() const;
    void setCost(double cost);
    double getCost();

private:
    GraphWidget *graph;
    double cost;
};

#endif // STATION_H

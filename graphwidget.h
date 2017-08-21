#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>


class Station;
class Edge;

class GraphWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);

private:
    void mouseDoubleClickEvent(QMouseEvent *event);

    QVector<Station *> stations;
    QVector<Edge *> edges;
};

#endif // GRAPHWIDGET_H

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;
class Station;
class Edge;

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);

private:
    void mouseDoubleClickEvent(QMouseEvent *evnt) override;
    void keyPressEvent(QKeyEvent *evnt) override;
    QPolygonF boundPolyg(QPointF pos0, QPointF pos1, qreal widthLine);

    QGraphicsView *vw;
    QGraphicsScene *scene;
    bool flagStation;
    bool flagEdge;
    bool flagShortest;
    bool flagCheapest;
    bool flagTransfer;
};

#endif // GRAPHWIDGET_H

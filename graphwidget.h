#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <set>

class Station;
class Edge;

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);

private:
    void mouseDoubleClickEvent(QMouseEvent *evnt);
    void keyPressEvent(QKeyEvent *evnt);

    std::set<Station *> stations;
    std::set<Edge *> edges;
    QGraphicsView *vw;
    QGraphicsScene *scene;
    bool flagStation;
    bool flagEdge;
    bool flagShortest;
    bool flagCheapest;
    bool flagTransfer;
};

#endif // GRAPHWIDGET_H

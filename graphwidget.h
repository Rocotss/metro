#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <set>
#include <vector>
#include <QWidget>

class QGraphicsView;
class QGraphicsScene;
class Station;
class Edge;

struct strDextra
{
    Edge *edge;
    qreal cost;
};

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
    void fDextra();

private:
    void mouseDoubleClickEvent(QMouseEvent *evnt) override;
    void keyPressEvent(QKeyEvent *evnt) override;
    QPolygonF boundPolyg(QPointF pos0, QPointF pos1, qreal widthLine);

    QGraphicsView *vw;
    QGraphicsScene *scene;
    bool flagStation;
    bool flagEdge;
    unsigned int flagCalc;
    std::set<Edge *> sEdges;
    std::vector<std::vector<strDextra>> vDextra;
};


#endif // GRAPHWIDGET_H

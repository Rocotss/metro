#include "graphwidget.h"
#include "station.h"
#include "edge.h"

#include <map>
#include <QMouseEvent>
#include <QRadioButton>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    QHBoxLayout *hbLayout = new QHBoxLayout();
    QGraphicsScene *scene= new QGraphicsScene(this);
    //hbLayout->addWidget();

    QVBoxLayout *vbtLayout = new QVBoxLayout();
    QRadioButton *createStations = new QRadioButton("Create stations");
    QRadioButton *createEdges = new QRadioButton("Create edges");
    QRadioButton *calcShortest = new QRadioButton("Seach chortest way");
    QRadioButton *calcCheapest = new QRadioButton("Seach cheapest way");
    QRadioButton *calcTransfers = new QRadioButton("Seach least of transfers way");
    vbtLayout->addWidget(createStations);
    vbtLayout->addWidget(createEdges);
    vbtLayout->addWidget(calcShortest);
    vbtLayout->addWidget(calcCheapest);
    vbtLayout->addWidget(calcTransfers);

    hbLayout->addLayout(vbtLayout);

    parent->setLayout(hbLayout);



    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200,-200,400,400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHints(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8),qreal(0.8));
    setMinimumSize(400,400);
    setWindowTitle("Metro");


}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if((event->button() & Qt::LeftButton) &&
            !QGraphicsView::itemAt(event->pos()))
    {

        stations.append(new Station(this));
        scene()->addItem(stations.last());
        stations.last()->setPos(mapToScene(event->pos()));
        update();
    }
}

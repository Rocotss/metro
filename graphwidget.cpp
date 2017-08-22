#include "graphwidget.h"
#include "station.h"
#include "edge.h"

#include <set>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QRadioButton>
#include <QPushButton>
#include <QGuiApplication>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), flagStation(true), flagEdge(true), flagShortest(true), flagCheapest(true), flagTransfer(true)
{
    scene= new QGraphicsScene(this);
    vw = new QGraphicsView(scene);

    QHBoxLayout *hblLayout = new QHBoxLayout();

    QVBoxLayout *vblPanel = new QVBoxLayout();
    QVBoxLayout *vblRb = new QVBoxLayout();

    QRadioButton *createStations = new QRadioButton("Create stations");
    createStations->setChecked(true);
    QRadioButton *createEdges = new QRadioButton("Create edges");
    QRadioButton *calcShortest = new QRadioButton("Seach chortest way");
    QRadioButton *calcCheapest = new QRadioButton("Seach cheapest way");
    QRadioButton *calcTransfers = new QRadioButton("Seach least of transfers way");

    QPushButton *btnSearch = new QPushButton("Search");
    btnSearch->setEnabled(false);

    QGroupBox *grbxPanel = new QGroupBox("Panel");

    vblRb->addWidget(createStations);
    vblRb->addWidget(createEdges);
    vblRb->addWidget(calcShortest);
    vblRb->addWidget(calcCheapest);
    vblRb->addWidget(calcTransfers);

    grbxPanel->setLayout(vblRb);

    vblPanel->addWidget(grbxPanel);
    vblPanel->addWidget(btnSearch);

    hblLayout->addWidget(vw);
    hblLayout->addLayout(vblPanel);
    setLayout(hblLayout);

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200,-200,400,400);
    vw->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    vw->setRenderHints(QPainter::Antialiasing);
    vw->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setMinimumSize(400,400);
    setWindowTitle("Metro");
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent *evnt)
{
    if((evnt->button() & Qt::LeftButton) &&
            scene->items(evnt->pos(), Qt::IntersectsItemShape).empty() &&
            flagStation == true)
    {

        Station *st = *stations.insert(new Station(this)).first;
        scene->addItem(st);
        st->setPos(vw->mapToScene(evnt->pos()));
        update();
    }
}
//dont work
void GraphWidget::keyPressEvent(QKeyEvent *evnt)
{
    if(evnt->key() == Qt::Key_Space)
    {
        qDebug()<<scene->selectedItems().count();
        if(scene->selectedItems().count() == 2)
        {
            Edge *edg = new Edge((Station *)scene->selectedItems().first(), (Station *)scene->selectedItems().last());
            scene->addItem(edg);
            update();
        }
    }
}

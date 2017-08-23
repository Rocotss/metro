#include "graphwidget.h"
#include "station.h"
#include "edge.h"

#include <set>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QRadioButton>
#include <QPushButton>
#include <QGuiApplication>
#include <QGroupBox>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), StationType(QGraphicsItem::UserType+2), EdgeType(QGraphicsItem::UserType+1),
      flagStation(true), flagEdge(true), flagShortest(true), flagCheapest(true), flagTransfer(true)
{
    scene= new QGraphicsScene(this);
    vw = new QGraphicsView(scene);

    QHBoxLayout *hblLayout = new QHBoxLayout();

    QVBoxLayout *vblPanel = new QVBoxLayout();
    QVBoxLayout *vblEdit = new QVBoxLayout();
    QVBoxLayout *vblSearch = new QVBoxLayout();

    QRadioButton *createStations = new QRadioButton("Create stations");
    createStations->setChecked(true);
    QRadioButton *createEdges = new QRadioButton("Create edges");
    QRadioButton *calcShortest = new QRadioButton("Seach chortest way");
    QRadioButton *calcCheapest = new QRadioButton("Seach cheapest way");
    QRadioButton *calcTransfers = new QRadioButton("Seach least of transfers way");

    QPushButton *btnSearch = new QPushButton("Search");
    btnSearch->setEnabled(false);

    QGroupBox *grbxEdit = new QGroupBox("Editing");
    QGroupBox *grbxSearch = new QGroupBox("Search");
    QButtonGroup *btnGroup = new QButtonGroup();

    btnGroup->addButton(createStations);
    btnGroup->addButton(createEdges);
    btnGroup->addButton(calcShortest);
    btnGroup->addButton(calcCheapest);
    btnGroup->addButton(calcTransfers);

    vblEdit->addWidget(createStations);
    vblEdit->addWidget(createEdges);
    grbxEdit->setLayout(vblEdit);

    vblSearch->addWidget(calcShortest);
    vblSearch->addWidget(calcCheapest);
    vblSearch->addWidget(calcTransfers);
    grbxSearch->setLayout(vblSearch);

    vblPanel->addWidget(grbxEdit);
    vblPanel->addWidget(grbxSearch);
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
/*
    QObject::connect(btnGroup, &QRadioButton::toggled, this, [](flagStation, flagEdge, flagShortest, flagCheapest, flagTransfer){


    });
*/
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent *evnt)
{
    if((evnt->button() & Qt::LeftButton) &&
            scene->items(evnt->pos(), Qt::IntersectsItemShape).empty() &&
            flagStation == true)
    {
        Station *st = new Station(this);
        scene->addItem(st);
        st->setPos(vw->mapToScene(evnt->pos()));
        update();
    }
}

void GraphWidget::keyPressEvent(QKeyEvent *evnt)
{
    if(evnt->key() == Qt::Key_Space)
    {
        if((scene->selectedItems().count() == 2)  &&
                (scene->selectedItems().first()->type() == StationType) &&
                (scene->selectedItems().last()->type() == StationType))
        {
            Edge *edg = new Edge((Station *)scene->selectedItems().first(), (Station *)scene->selectedItems().last());
            scene->addItem(edg);
            update();
        }
    }

    if(evnt->key() == Qt::Key_Delete &&
            scene->selectedItems().count() == 1)
    {
        QGraphicsItem *data = scene->selectedItems().last();
        scene->removeItem(data);
        delete data;
    }
}

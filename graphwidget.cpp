#include "graphwidget.h"
#include "station.h"
#include "edge.h"

#include <math.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QRadioButton>
#include <QPushButton>
#include <QGuiApplication>
#include <QGroupBox>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>

#include <QDebug>

enum{EdgeType = QGraphicsItem::UserType + 1, StationType};

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent),
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

/*    QObject::connect(btnGroup, &QRadioButton::toggled, this, [](){
        int id = QButtonGroup.checkedId();
        switch (id) {
        case 0:
            {
                flagStation = true;
                flagEdge = false;
                flagShortest = false;
                flagCheapest = false;
                flagTransfer = false;
                break;
            }
        case 1:
            {
                flagStation = false;
                flagEdge = true;
                flagShortest = false;
                flagCheapest = false;
                flagTransfer = false;
                break;
            }
        case 2:
            {
                flagStation = false;
                flagEdge = false;
                flagShortest = true;
                flagCheapest = false;
                flagTransfer = false;
                break;
            }
        case 3:
            {
                flagStation = false;
                flagEdge = false;
                flagShortest = false;
                flagCheapest = true;
                flagTransfer = false;
                break;
            }
        case 4:
            {
                flagStation = false;
                flagEdge = false;
                flagShortest = false;
                flagCheapest = false;
                flagTransfer = true;
                break;
            }
        default:
            break;
        }
    });*/

}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent *evnt)
{
    if((evnt->button() & Qt::LeftButton) &&
            scene->items(evnt->pos(), Qt::IntersectsItemShape).empty() &&
            flagStation)
    {
        Station *st = new Station(this);
        scene->addItem(st);
        st->setPos(vw->mapToScene(evnt->pos()));
        update();
    }
}

void GraphWidget::keyPressEvent(QKeyEvent *evnt)
{
    //(5) проверякм, что нажат пробел
    if(evnt->key() == Qt::Key_Space &&
            flagEdge)
    {
        QList<QGraphicsItem *> listStation = scene->selectedItems();

        //(4) проверяем, что прогон рисуется между станциями
        if((listStation.count() == 2)  &&
                (listStation.first()->type() == StationType) &&
                (listStation.last()->type() == StationType))
        {
             //(3) проверям, что прогон не перескается с другими прогонами
            bool change = true;
            QList<QGraphicsItem *> edgeIntersect = scene->items(boundPolyg(listStation.first()->pos(), listStation.last()->pos(), Edge::penWidth), Qt::IntersectsItemShape);
            unsigned coutEdge = 0;

            for(auto searchEdge = edgeIntersect.begin(); searchEdge != edgeIntersect.end(); ++searchEdge)
            {
                if((*searchEdge)->type() == EdgeType)
                {
                    ++coutEdge;
                    change = false;
                }
            }
            // пока что не работает, т.к. длинна проверяющего прямоугольника больше чем нужно
            if(true)
            {
                // (2.5) проверяем, что прогонов меньше пяти
                if(coutEdge < 5)
                {
                    Edge *edg = new Edge(static_cast<Station *>(listStation.first()), static_cast<Station *>(listStation.last()));
                    scene->addItem(edg);
                    update();

                    //(2) проверка стала ли станция пересадочной
                    for(int i=0; i < 2; ++i)
                    {
                        auto iter = listStation.begin();

                        //(1) проверяем что станция не пересадочная
                        if((static_cast<Station *>(*iter))->getCost() == 0)
                        {
                            QList<QGraphicsItem *> listItem = scene->items((static_cast<Station *>(*iter))->pos()+QPointF(10,10));
                            unsigned int count = 0;
                            qDebug()<<(*listItem.begin())->type()<<listItem.size();

                            //(0) задаём цену станции
                            for(auto iterItem = listItem.begin(); iterItem != listItem.end(); ++iterItem)
                            {
                                if((*iterItem)->type() == EdgeType) // считаем количество присоеденённых к станции прогонов
                                {
                                    ++count;
                                }
                            }

                            if(count > 2)
                            {
                                unsigned    int dInput = QInputDialog::getInt(this, "Input cost", "Enter the price of the transfer station", 1.0);
                                if(dInput <= 0.0)
                                {
                                    dInput = 1;
                                    QMessageBox msgBox;
                                    msgBox.setText("Cost dont must <0 ! Cost = 1.");
                                    msgBox.exec();
                                }

                                (static_cast<Station *>(*iter))->setCost(dInput);
                                ++iter;
                            }
                            //(0)

                        } // (1)
                    } //(2)
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText("Can not attach more than five edges");
                    msgBox.exec();
                } //(2.5)
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText(" Edges dont intesect!!");
                msgBox.exec();
            }
            //(3)

        } //(4)
    } //(5)

    // удвление объекта сцены
    if(evnt->key() == Qt::Key_Delete &&
            scene->selectedItems().count() == 1)
    {
        QGraphicsItem *data = scene->selectedItems().last();
        scene->removeItem(data);
        delete data;
    }
}

QPolygonF GraphWidget::boundPolyg(QPointF point0, QPointF point1, qreal widthLine)
{
    QPointF coner1;
    QPointF coner2;
    QPointF coner3;
    QPointF coner4;
    QPolygonF polyg;

    // ToDo: уменьшить область поиска пересечений до края станции

    qreal gip = sqrt(pow(point0.x() - point1.x(), 2) + pow(point0.y() - point1.y(), 2));

    coner1.setX(point1.x() - (widthLine/2 * (point0.x() - point1.x()) / gip));
    coner1.setY(point1.y() + (widthLine/2 * (point0.y() - point1.y()) / gip));

    coner2.setX(point1.x() + (widthLine/2 * (point0.x() - point1.x()) / gip));
    coner2.setY(point1.y() - (widthLine/2 * (point0.x() - point1.x()) / gip));

    coner3.setX(point0.x() + (widthLine/2 * (point1.x() - point0.x()) / gip));
    coner3.setY(point0.y() - (widthLine/2 * (point1.x() - point0.x()) / gip));

    coner4.setX(point0.x() - (widthLine/2 * (point1.x() - point0.x()) / gip));
    coner4.setY(point0.y() + (widthLine/2 * (point1.y() - point0.y()) / gip));

    polyg<<coner1<<coner2<<coner3<<coner4;

    return polyg;
}

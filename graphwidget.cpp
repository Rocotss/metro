#include "graphwidget.h"
#include "station.h"
#include "edge.h"

#include <set>
#include <math.h>
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
    if(evnt->key() == Qt::Key_Space &&
            flagEdge)
    {
        QList<QGraphicsItem *> listStation = scene->selectedItems();

        if((listStation.count() == 2)  &&
                (listStation.first()->type() == StationType) &&
                (listStation.last()->type() == StationType))
        {
            QPointF st1 = listStation.first()->pos();
            QPointF st2 = listStation.last()->pos();
            qreal h = 7.5 * (st1.y() - st2.y()) / sqrt(pow((st1.x() - st2.x()), 2) + pow((st1.y() - st2.y()), 2));
            qreal w = sqrt(pow(7.5, 2) - pow(h, 2));

            //if()


            if(true)//scene->items().empty()) // проверям, что прогон не перескается с другими объектами
            {
                Edge *edg = new Edge((Station *)listStation.first(), (Station *)listStation.last());
                scene->addItem(edg);
                update();

                // проверка стала ли станция пересадочной
                for(int i=0; i < 2; ++i)
                {
                    auto iter = listStation.begin();

                    if((static_cast<Station *>(*iter))->getCost() == 0) // проверяем что станция не пересадочная
                    {
                        QList<QGraphicsItem *> listItem = scene->items((static_cast<Station *>(*iter))->pos()+QPointF(10,10));
                        unsigned int count = 0;
                        qDebug()<<(*listItem.begin())->type()<<listItem.size();

                        for(auto iterItem = listItem.begin(); iterItem != listItem.end(); ++iterItem)
                        {
                            if((*iterItem)->type() == EdgeType) // считаем количество присоеденённых к станции прогонов
                            {
                                ++count;
                            }
                        }

                        // задаём цену станции
                        if(count > 2)
                        {
                            double dInput = QInputDialog::getDouble(this, "Input cost", "Enter the price of the transfer station", 1.0);
                            if(dInput <= 0.0)
                            {
                                dInput = 1;
                                QMessageBox *msgBox = new QMessageBox();
                                msgBox->setText("Cost dont must <0 ! Cost = 1.");
                                msgBox->exec();
                            }

                            (static_cast<Station *>(*iter))->setCost(dInput);
                            ++iter;
                        }
                    }
                }
            }
            else
            {
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setText(" Edges dont intesect!!");
                msgBox->exec();
            }
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

#include <QGraphicsView>
#include "tabcanvas.h"
#include "diagram/Diagrams.h"

using namespace SceneType;

/**
 *
 * @param parent
 * @param type
 */
TabCanvas::TabCanvas(QWidget *parent, DiagramType type) :
        QWidget(parent) {

    create_scene();
    layout = new QVBoxLayout();
    layout->addWidget(view);
    setLayout(layout);

    // TOOD: create Diagram depending on the type passed;
    switch (type) {
        case DiagramType::SEQUENCE:
            diagram = new SequenceDiagram();
            break;
        case DiagramType::CLASS:
            diagram = new ClassDiagram();
            break;
        default:
            throw std::runtime_error("Unknown diagram type");
    }
}

TabCanvas::~TabCanvas() {
    delete diagram;
    delete layout;
}

/**
 *
 */
void TabCanvas::create_scene() {
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);

    scene->setSceneRect(0, 0, 1400, 1000);
    view->setMinimumSize(400, 400);

    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
}

Object *TabCanvas::selectedObject() {
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Object *>(items.first());
    } else {
        return 0;
    }
}

/**
 *
 */
void TabCanvas::addEntity() {
    diagram->addEntity(scene);
}

/**
 *
 */
void TabCanvas::addConnection() {
    diagram->addConnection(scene);
}

/**
 *
 */
void TabCanvas::remove() {
    QList<QGraphicsItem *> items = scene->selectedItems();
    qDeleteAll(items);
}

/**
 *
 */
void TabCanvas::cut() {
    qDebug() << "cut";
    // TODO: implement me
}

/**
 *
 */
void TabCanvas::copy() {
    qDebug() << "copy";
    Object *tmp = selectedObject();
    if(!tmp)
        return;

//    QString str = QString("Actor %1 %2 %3 %4")
//                    .arg(tmp->x())
//                    .arg(tmp->y())
//                    .arg(tmp->scale())
    // TODO: implement me
}

/**
 *
 */
void TabCanvas::paste() {
    qDebug() << "paste";
    // TODO: paste
}

/**
 *
 */
void TabCanvas::properties() {
    qDebug() << "not segfault";
    diagram->setProperties();
}

/**
 *
 */
void TabCanvas::sendToBack() {
    qDebug() << "not segfault";
    diagram->sendToBack();
}

void TabCanvas::sendToFront() {
    qDebug() << "not segfault";
    diagram->sendToFront();
}

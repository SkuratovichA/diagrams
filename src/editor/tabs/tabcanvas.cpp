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

/**
 *
 */
void TabCanvas::addEntity() {
    scene->addItem(diagram->addEntity());
}

/**
 *
 */
void TabCanvas::addConnection() {
    scene->addItem(diagram->addConnection());
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

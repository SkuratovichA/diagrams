#include <QGraphicsView>
#include <QUndoStack>
#include <QUndoGroup>
#include "tabcanvas.h"
#include "diagram/Commands.h"

using namespace SceneType;

class editorInterface;

 /**
 *
 * @param parent
 * @param type
 */
TabCanvas::TabCanvas(QWidget *parent, DiagramType type, QUndoGroup *parentGroup)
{
    qDebug() << "Here?? Why...";
    undoStack = new QUndoStack(parentGroup);

    createScene();

    // TODO: create Diagram depending on the type passed;
    switch (type) {
        case DiagramType::SEQUENCE:
            qDebug() << "newEntity type sequence";
            newEntityType = DiagramItem::Actor;
            newConnectionType = DiagramItem::ActorConnection;
            //diagram = new SequenceDiagram();
            break;
        case DiagramType::CLASS:
            qDebug() << "newEntity type class";
            newEntityType = DiagramItem::Class;
            newConnectionType = DiagramItem::ClassConnection;
            //diagram = new ClassDiagram();
            break;
        default:
            throw std::runtime_error("Unknown diagram type");
    }
}

/**
 *
 */
TabCanvas::~TabCanvas() {
    //delete diagram;
    delete layout;
}

/**
 *
 */
void TabCanvas::createScene() {
    editorScene = new EditorScene(this);
    qDebug() << "Scene after creating is " << editorScene;
    //QBrush pixmapBrush(QPixmap(":/icons/background.png").scaled(30,30));
    //editorScene->setBackgroundBrush(pixmapBrush);
    editorScene->setSceneRect(QRect(0, 0, 1400, 1400));

    connect(editorScene, &EditorScene::itemMoved, this, &TabCanvas::moveEntity);

    auto *view = new QGraphicsView(editorScene);
    setCentralWidget(view);
    //resize(1400, 1400);
   /*  TODO: maybe add this instead of the two lines above?
    */
    //layout = new QVBoxLayout();
    //layout->addWidget(view);
    //setLayout(layout);



//    scene = new QGraphicsScene();
//    view = new QGraphicsView(scene);
//
//    scene->setSceneRect(0, 0, 1400, 1000);
//    view->setMinimumSize(400, 400);
//
//    view->setScene(scene);
//    view->setDragMode(QGraphicsView::RubberBandDrag);
//    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
}

/**
 *
 */
void TabCanvas::moveEntity(DiagramItem *movedItem, const QPointF &startPosition) {
    qDebug() << "Some troubles mr Andrei" << movedItem << startPosition;
    undoStack->push(new MoveCommand(movedItem, startPosition));
    qDebug() << "Some troubles mr Sasha";
}

/**
 *
 */
void TabCanvas::removeEntity() {
    if (editorScene->selectedItems().isEmpty()) {
        return;
    }
    QUndoCommand *deleteCommand = new DeleteCommand(editorScene);
    undoStack->push(deleteCommand);
//    QList<QGraphicsItem *> items = editorScene->selectedItems();
//    qDeleteAll(items);
}

/**
 *
 */
void TabCanvas::addEntity() {
    qDebug() << "add entity in tabcanvas.cpp";
    QUndoCommand *addCommand = new AddEntityCommand(newEntityType, editorScene);
    undoStack->push(addCommand);
//    diagram->addEntity(editorScene);
}

/**
 *
 */
void TabCanvas::addConnection() {
    QUndoCommand *addConnection = new AddEntityCommand(newConnectionType, editorScene);
    undoStack->push(addConnection);
    //diagram->addConnection(editorScene);
}

/**
 *
 * @return
 */
QGraphicsItem *TabCanvas::selectedObject() {
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<QGraphicsItem *>(items.first());
    } else {
        return 0;
    }
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
void TabCanvas::cut() {
    qDebug() << "cut";
    // TODO: implement me
}

/**
 *
 */
void TabCanvas::copy() {
//    qDebug() << "copy";
//    Object *tmp = selectedObject();
 //    if(!tmp)
//        return;

//    QString str = QString("DiagramItem %1 %2 %3 %4")
//                    .arg(tmp->x())
//                    .arg(tmp->y())
//                    .arg(tmp->scale())
    // TODO: implement me
}

/**
 *
 */
void TabCanvas::properties() {
    qDebug() << "not segfault";
    //diagram->setProperties();
}

/**
 *
 */
void TabCanvas::sendToBack() {
    qDebug() << "not segfault";
    //diagram->sendToBack();
}

/**
 *
 */
void TabCanvas::sendToFront() {
    qDebug() << "not segfault";
    //diagram->sendToFront();
}

/**
 *
 * @return
 */
std::string TabCanvas::getStringRepresentation() {
    // for every object, return a class in json?
    return {"hello"};
}

/** When tab is changed, there is a need to manually set the current undo stack.
 *
 * @return undo stack
 */
QUndoStack* TabCanvas::getUndoStack() {
    return undoStack;
}

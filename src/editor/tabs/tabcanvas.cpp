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
TabCanvas::TabCanvas(QWidget *parent, DiagramType type, QUndoGroup *parentGroup) {
    undoStack = new QUndoStack(parentGroup);
    createScene();
    if (type != DiagramType::CLASS && type != DiagramType::SEQUENCE) {
        throw std::runtime_error("Unknown diagram type");
    }
    this->type = type;
}

/**
 *
 */
TabCanvas::~TabCanvas() {
    delete layout;
}

/**
 *
 */
void TabCanvas::createScene() {
    editorScene = new EditorScene(this);

    editorScene->setSceneRect(QRect(0, 0, 800, 800));
    (void)connect(editorScene, &EditorScene::itemMoved, this, &TabCanvas::moveEntity);
    auto *view = new QGraphicsView(editorScene);

    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing
                         | QPainter::TextAntialiasing);

    setCentralWidget(view);
}

/**
 *
 */
void TabCanvas::moveEntity(ActorDiagramItem *movedItem, const QPointF &startPosition) {
    undoStack->push(new MoveCommand(movedItem, startPosition));
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
}

/**
 *
 */
void TabCanvas::addEntity() {
    QUndoCommand *addCommand = nullptr;
    switch (type) {
        case DiagramType::SEQUENCE:
            addCommand = new AddActorCommand(editorScene);
            break;
        case DiagramType::CLASS:
            addCommand = new AddClassCommand(editorScene);
            break;
        default:
            assert(!"This statement must not be reached");
            return;
    }
    undoStack->push(addCommand);
}

/**
 *
 */
void TabCanvas::addConnection() {
    QUndoCommand *connectionCommand = nullptr;

    if (type == DiagramType::SEQUENCE) {
//      TODO: add line connection for a sequence diagram
//      connectionCommand = new AddActorConnectionCommand(editorScene);
        qDebug() << "add nodes for a sequence diagram";
        return;
    } else if (type == DiagramType::CLASS) {
        auto nodes = getSelectedDiagramItems<ClassDiagramItem>();
        qDebug() << "still alive";
        auto emptySelect = nodes == QPair<ClassDiagramItem *, ClassDiagramItem*>();
        if (emptySelect) {
            return;
        }
        connectionCommand = new AddClassConnectionCommand(nodes.first, nodes.second, ClassConnectionItem::Aggregation,
                                                          editorScene);
    } else {
        assert(!"There are only two types: SEQUENCE and CLASS.");
    }
    undoStack->push(connectionCommand);
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
        return nullptr;
    }
}

/**
 *
 */
void TabCanvas::paste() {
    qDebug() << "paste";
    // FIXME: paste
}

/**
 *
 */
void TabCanvas::cut() {
    qDebug() << "cut";
    // FIXME: implement me
}

/**
 *
 */
void TabCanvas::copy() {
    // TODO: implement me
#if 0
    qDebug() << "copy";
    Object *tmp = selectedObject();
        if(!tmp)
        return;

    QString str = QString("ActorDiagramItem %1 %2 %3 %4")
                    .arg(tmp->x())
                    .arg(tmp->y())
                    .arg(tmp->scale())
#endif
}

/**
 *
 */
void TabCanvas::properties() {
    qDebug() << "properties";
}

/**
 *
 */
void TabCanvas::sendToBack() {
    qDebug() << "send to back";
}

/**
 *
 */
void TabCanvas::sendToFront() {
    qDebug() << "send to front";
}

/**
 *
 * @return
 */
std::string TabCanvas::getStringRepresentation() {
    // FIXME: in the end
    return {"hello"};
}

/** When tab is changed, there is a need to manually set the current undo stack.
 *
 * @return undo stack
 */
QUndoStack *TabCanvas::getUndoStack() {
    return undoStack;
}

template<typename T>
QPair<T *, T *> TabCanvas::getSelectedDiagramItems() {
    auto items = editorScene->selectedItems();
    if (items.isEmpty()) {
        return QPair<T*, T*>();
    }
    auto first = dynamic_cast<T *>(items.first());
    T *rest = nullptr;

    switch (items.count()) {
        case 2:
            items.removeFirst();
            // fallthrough
        case 1:
            rest = dynamic_cast<T *>(items.first());
            if (first == nullptr || rest == nullptr) {
                qDebug() << "shit fuck";
                return QPair<T*, T*>();
            }
            break;
        default:
            qDebug() << items.count() << " nodes were selected. Bad";
            return QPair<T*, T*>();
    }
    // first is always T*, the rest is always QVector
    return QPair<T *, T *>(first, rest);
}
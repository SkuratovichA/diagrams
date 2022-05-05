#include <QGraphicsView>
#include <QUndoStack>
#include <QUndoGroup>
#include "tabcanvas.h"
#include "diagram/Commands.h"
#include <QRandomGenerator>
#include <QColor>

using namespace SceneType;

class editorInterface;

#define ADD_SIGNAL(obj, name, icon, shortcut, receiver, memberslot) \
    do {                                                          \
        obj = new QAction(tr((name)), this);                      \
        /*obj->setIcon(icon);*/                                   \
        /*(obj)->setShortcut(tr(shortcut));*/                     \
        connect((obj), SIGNAL(triggered()), receiver, memberslot);\
    } while(0)

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

    buffer = new ItemsBuffer();

    createMenusClass();
    createMenusConnections();
    // TODO: do same for sequence diagram
}

/**
 *
 */
TabCanvas::~TabCanvas() {
    delete buffer;
    delete editorScene;
}

void TabCanvas::createMenusClass() {
    ADD_SIGNAL(addMethod, "Add &Method",       "+", "+", this, SLOT(addMethod_triggered()));
    ADD_SIGNAL(rmMethod,  "Delete &Method",    "+", "+", this, SLOT(rmMethod_triggered()));
    ADD_SIGNAL(addAttr,   "Add &Attribute",    "+", "+", this, SLOT(addAttr_triggered()));
    ADD_SIGNAL(rmAttr,    "Delete &Attribute", "+", "+", this, SLOT(rmAttr_triggered()));

    classMenu = new QMenu();
    classMenu->addAction(addMethod);
    classMenu->addAction(rmMethod);
    classMenu->addAction(addAttr);
    classMenu->addAction(rmAttr);
}

void TabCanvas::createMenusConnections() {
    ADD_SIGNAL(aggregation,    "Aggregation &Relation",    "+", "+", this, SLOT(aggregation_triggered()));
    ADD_SIGNAL(composition,    "Composition &Relation",    "+", "+", this, SLOT(composition_triggered()));
    ADD_SIGNAL(generalization, "Generalization &Relation", "+", "+", this, SLOT(generalization_triggered()));
    ADD_SIGNAL(association,    "Association &Relation",    "+", "+", this, SLOT(association_triggered()));

    connectionMenu = new QMenu();
    connectionMenu->addAction(aggregation);
    connectionMenu->addAction(composition);
    connectionMenu->addAction(generalization);
    connectionMenu->addAction(association);
}

void TabCanvas::ShowContextMenu(const QPoint& pos) // this is a slot
{
    QGraphicsItem *item = selectedObject();
    ClassDiagramItem *ptr1;
    ActorDiagramItem *ptr2;

    if (item == nullptr) {
        return;
    }

    switch (type) {
        case SceneType::CLASS:
            ptr1 = dynamic_cast<ClassDiagramItem *>(item);

            if (ptr1 != nullptr) {
                classMenu->exec(this->mapToGlobal(pos));
            }
            else {
                connectionMenu->exec(this->mapToGlobal(pos));
            }

            break;
        case SceneType::SEQUENCE:
            ptr2 = dynamic_cast<ActorDiagramItem *>(item);

            if (ptr2 != nullptr) {
                // TODO: actor
            }
            else {
                // TODO: message
            }

            break;
        default:
            assert(!"This statement must not be reached");
    }
}

/**
 *
 */
void TabCanvas::createScene() {
    editorScene = new EditorScene(this);

    editorScene->setSceneRect(QRect(0, 0, 800, 800));
    (void)connect(editorScene, &EditorScene::itemMoved, this, &TabCanvas::moveEntity);
    view = new QGraphicsView(editorScene);

    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing
                         | QPainter::TextAntialiasing);
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(ShowContextMenu(const QPoint&)));

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
    QList<QString> attrs;
    QList<QString> methods;
    QPoint point = generateCoords();

    switch (type) {
        case DiagramType::SEQUENCE:
            createActor = new actorParams(point.x(), point.y() ,1.0, "_ACTOR_",
                                          generateColor(), 70, 110);
            addCommand = new AddActorCommand(editorScene, createActor);
            delete createActor;

            break;
        case DiagramType::CLASS:
            attrs.push_back("+ int name");
            methods.push_back("+ int name()");

            createItem = new classParams(point.x(), point.y(), 1.0,"_NAME_",
                                         generateColor(), 120.0, 120.0, attrs, methods);
            addCommand = new AddClassCommand(editorScene, createItem);
            delete createItem;

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
    switch (type) {
        case SceneType::SEQUENCE:
            for (auto ptr : buffer->actorItems()) {
                ActorDiagramItem *diagramItem = new ActorDiagramItem(ptr);
                diagramItem->setPos(ptr->x(), ptr->y());
                editorScene->addItem(diagramItem);
            }
            break;
        case SceneType::CLASS:
            for (auto ptr : buffer->classItems()) {
                ClassDiagramItem *diagramItem = new ClassDiagramItem(ptr);
                diagramItem->setPos(ptr->x(), ptr->y());
                editorScene->addItem(diagramItem);
            }
            break;
        default:
            qDebug() << "It is impossible";
    }
    editorScene->update();
}

/**
 *
 */
void TabCanvas::cut() {
    copy();
    QList<QGraphicsItem *> items = editorScene->selectedItems();

    for (auto val : items) {
        editorScene->removeItem(val);
    }
}

/**
 *
 */
void TabCanvas::copy() {
    ClassDiagramItem *ptrClass;
    ActorDiagramItem *ptrActor;
    QList<QGraphicsItem *> items = editorScene->selectedItems();

    buffer->clearBuffer();
    switch (type) {
        case SceneType::SEQUENCE:
            for (auto val : items) {
                ptrActor = dynamic_cast<ActorDiagramItem *>(val);
                if (ptrActor != nullptr) {
                    buffer->fillActorItems(ptrActor);
                }
            }
            break;
        case SceneType::CLASS:
            for (auto val : items) {
                ptrClass = dynamic_cast<ClassDiagramItem *>(val);
                if (ptrClass != nullptr) {
                    buffer->fillClassItems(ptrClass);
                }
            }
            break;
        default:
            qDebug() << "It is impossible";
    }
}

void TabCanvas::scaleView(qreal scaleFactor) {
    qreal factor = view->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    view->scale(scaleFactor, scaleFactor);
}

/**
 *
 */
void TabCanvas::zoomIn() {
    scaleView(qreal(1.2));
}

/**
 *
 */
void TabCanvas::zoomOut() {
    scaleView(1 / qreal(1.2));
}

/**
 *
 */
void TabCanvas::addMethod_triggered() {
    ClassDiagramItem *item = dynamic_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() + item->rowHeight());

    auto line = item->createLine(0, item->height());
    item->pushMethodLine(line);

    //auto text = item->createText(item->tabText(), item->height() + item->tabText(), "+ int example()");
    CustomAttrText *text = new CustomAttrText(item, "+ int example()", item->tabText(), item->height() + item->tabText(), item->flags());
    item->pushMethod(text);

    item->setHeight(item->height() + item->rowHeight());
    qDebug() << "add Method";
};

/**
 *
 */
void TabCanvas::rmMethod_triggered() {
    ClassDiagramItem *item = dynamic_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    // one default METHODS
    auto size = item->methods().size();
    if (size < 2) {
        qDebug() << "No methods";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() - item->rowHeight());

    // delete line and text
    item->popMethod();
    item->popMethodsLine();

    item->setHeight(item->height() - item->rowHeight());
    qDebug() << "delete Method";
};

/**
 *
 */
void TabCanvas::addAttr_triggered() {
    ClassDiagramItem *item = dynamic_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() + item->rowHeight());

    long long inc = item->attrs().size() + 1;
    item->moveTexts(1, inc);
    item->moveLines(1, inc);

    auto line = item->createLine(0, inc * item->rowHeight());
    item->pushAttrLine(line);

    //auto text = item->createText(item->tabText(), item->rowHeight() * inc + item->tabText(), "+ int word");
    CustomAttrText *text = new CustomAttrText(item, "+ int word", item->tabText(), item->rowHeight() * inc + item->tabText(), item->flags());
    item->pushAttr(text);

    item->setHeight(item->height() + item->rowHeight());
    qDebug() << "add Attr";
};

/**
 *
 */
void TabCanvas::rmAttr_triggered() {
    ClassDiagramItem *item = dynamic_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    auto size = item->attrs().size();
    qDebug() << size;
    if (size < 1) {
        qDebug() << "No methods";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() - item->rowHeight());

    // delete line and text
    item->popAttr();
    item->popAttrsLine();

    // move items
    long long inc = item->attrs().size() + 1;
    item->moveTexts(0, inc);
    item->moveLines(0, inc);

    //item->setPos(tmp_x, tmp_y);
    item->setHeight(item->height() - item->rowHeight());
    qDebug() << "delete Attr";
};

void TabCanvas::aggregation_triggered() {
    return;
}

void TabCanvas::composition_triggered() {
    return;
}

void TabCanvas::generalization_triggered() {
    return;
}

void TabCanvas::association_triggered() {
    return;
}

/**
 *
 * @return
 */
std::string TabCanvas::getStringRepresentation() {
    // FIXME: in the end
    return {"hello"};
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

void TabCanvas::sendToBack() {

}

void TabCanvas::sendToFront() {

}

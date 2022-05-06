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
    createMenusSequence();
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
    ADD_SIGNAL(dependency,     "Dependency &Relation",     "+", "+", this, SLOT(dependency_triggered()));
    ADD_SIGNAL(orientation,    "Change &orientation",      "+", "+", this, SLOT(orientation_triggered()));

    connectionMenu = new QMenu();
    connectionMenu->addAction(aggregation);
    connectionMenu->addAction(composition);
    connectionMenu->addAction(generalization);
    connectionMenu->addAction(association);
    connectionMenu->addAction(dependency);
    connectionMenu->addAction(orientation);
}

void TabCanvas::createMenusSequence() {
    void request_triggered();
    void response_triggered();
    void deletion_triggered();
    void creation_triggered();
    ADD_SIGNAL(requestMsg,  "Request &Message", "+", "+", this, SLOT(request_triggered()));
    ADD_SIGNAL(responseMsg, "Response &Message", "+", "+", this, SLOT(response_triggered()));
    ADD_SIGNAL(createMsg,   "Create &Message", "+", "+", this, SLOT(creation_triggered()));
    ADD_SIGNAL(deleteMsg,   "Delete &Message", "+", "+", this, SLOT(deletion_triggered()));

    sequenceMenu = new QMenu();
    sequenceMenu->addAction(requestMsg);
    sequenceMenu->addAction(responseMsg);
    sequenceMenu->addAction(createMsg);
    sequenceMenu->addAction(deleteMsg);
}

// I CHANGE THIS FUNCTION FOR SEQUENCE DIAGRAM
void TabCanvas::ShowContextMenu(const QPoint& pos) // this is a slot
{
    QList<QGraphicsItem *>items = editorScene->selectedItems();
    qDebug() << items.count();
    ClassDiagramItem *ptr1;
    ActorDiagramItem *ptr2;
    ActorDiagramItem *ptr3;

    if (items.isEmpty()) {
        return;
    }

    QGraphicsItem *item = items.first();

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
            if (items.count() < 2) {
                return;
            }
            qDebug() << "apapapa";
            ptr2 = dynamic_cast<ActorDiagramItem *>(items.takeAt(0));
            ptr3 = dynamic_cast<ActorDiagramItem *>(items.takeAt(0));

            if (ptr2 == nullptr || ptr3 == nullptr) {
                return;
            }

            sequenceMenu->exec(this->mapToGlobal(pos));

            break;
        default:
            assert(!"This statement must not be reached");
    }
}
////////////////////////////////////////////////////////////

/**
 *
 */
void TabCanvas::createScene() {
    editorScene = new EditorScene(this);

    editorScene->setSceneRect(QRect(0, 0, 800, 800));
    (void)connect(editorScene, &EditorScene::itemMoved, this, &TabCanvas::moveEntity);
    view = new QGraphicsView(editorScene);

    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
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
    static uint64_t actorStep = 0;
    QPoint point = generateCoords();

    switch (type) {
        case DiagramType::SEQUENCE:
            createActor = new actorParams(point.x(), point.y() , "ACTOR",
                                          generateColor(), 80, 50);
            addCommand = new AddActorCommand(editorScene, createActor);
            delete createActor;

            break;
        case DiagramType::CLASS:
            attrs.push_back("+ int name");
            methods.push_back("+ int name()");

            createItem = new classParams(point.x(), point.y(), "NAME",
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
        auto emptySelect = nodes == QPair<ClassDiagramItem *, ClassDiagramItem*>();
        if (emptySelect) {
            return;
        }
        connectionCommand = new AddClassConnectionCommand(nodes.first, nodes.second, ClassConnectionItem::Dependency,
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
    ClassTextAttr *text = new ClassTextAttr(item, "+ int example()", item->tabText(), item->height() + item->tabText(), item->flags());
    item->pushMethod(text);

    item->setHeight(item->height() + item->rowHeight());
    for (auto x : item->connections()) {
        x->trackNodes();
    }
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
    for (auto x : item->connections()) {
        x->trackNodes();
    }
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
    ClassTextAttr *text = new ClassTextAttr(item, "+ int word", item->tabText(), item->rowHeight() * inc + item->tabText(), item->flags());
    item->pushAttr(text);

    item->setHeight(item->height() + item->rowHeight());
    for (auto x : item->connections()) {
        x->trackNodes();
    }
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
    for (auto x : item->connections()) {
        x->trackNodes();
    }
    qDebug() << "delete Attr";
};

void TabCanvas::aggregation_triggered() {
    auto line = dynamic_cast<ClassConnectionItem *>(selectedObject());
    line->setType(ClassConnectionItem::Aggregation);
    editorScene->update();
}

void TabCanvas::composition_triggered() {
    auto line = dynamic_cast<ClassConnectionItem *>(selectedObject());
    line->setType(ClassConnectionItem::Composition);
    editorScene->update();
}

void TabCanvas::generalization_triggered() {
    auto line = dynamic_cast<ClassConnectionItem *>(selectedObject());
    line->setType(ClassConnectionItem::Generalization);
    editorScene->update();
}

void TabCanvas::association_triggered() {
    auto line = dynamic_cast<ClassConnectionItem *>(selectedObject());
    line->setType(ClassConnectionItem::Association);
    editorScene->update();
}

void TabCanvas::dependency_triggered() {
    auto line = dynamic_cast<ClassConnectionItem *>(selectedObject());
    line->setType(ClassConnectionItem::Dependency);
    editorScene->update();
}

void TabCanvas::orientation_triggered() {
    auto line = dynamic_cast<ClassConnectionItem *>(selectedObject());
    line->changeOrientation();
    editorScene->update();
}
//////////////////////////////////////////////////
void TabCanvas::request_triggered() {
    auto a = editorScene->selectedItems();
    qDebug() << "num of selected items: " << a.count();
    auto node1 = dynamic_cast<ActorDiagramItem *>(a.takeAt(0));
    auto node2 = dynamic_cast<ActorDiagramItem *>(a.takeAt(0));
    auto line = new ActorConnectionItem(node1, node2, ActorConnectionItem::Request);
    editorScene->update();
    qDebug() << "request triggered";
}

void TabCanvas::response_triggered() {
    qDebug() << "response triggered";
}

void TabCanvas::deletion_triggered() {
    qDebug() << "deletion triggered";
}

void TabCanvas::creation_triggered() {
    qDebug() << "creation triggered";
}
//////////////////////////////////////////////////

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
    QList<T *> listl;
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
            foreach(auto item, items) {
                if (dynamic_cast<T *>(item) != nullptr) {
                    listl.append(dynamic_cast<T* >(item));
                }
            }
            qDebug() << listl.count() << " elements to select.";
            first = listl.first();
            listl.removeFirst();
            rest = listl.first();
    }
    // first is always T*, the rest is always QVector
    return QPair<T *, T *>(first, rest);
}

void TabCanvas::setZvalue(int forSelect, int forOther) {
    QList<QGraphicsItem *> selItems = editorScene->selectedItems();
    QList<QGraphicsItem *> allItems = editorScene->items();

    for (auto x : selItems) {
        allItems.removeOne(x);
        x->setZValue(forSelect);
    }

    ClassDiagramItem *ptr1;
    ActorDiagramItem *ptr2;

    for (auto x : allItems) {
        ptr1 = dynamic_cast<ClassDiagramItem *>(x);
        ptr2 = dynamic_cast<ActorDiagramItem *>(x);

        if (ptr1 == nullptr && ptr2 == nullptr) {
            continue;
        }
        x->setZValue(forOther);
    }
    editorScene->update();
}

void TabCanvas::toBack() {
    setZvalue(-1, 1);
}

void TabCanvas::toFront() {
    setZvalue(1, -1);
}


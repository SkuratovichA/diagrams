#include <QGraphicsView>
#include <QUndoStack>
#include <QColor>
#include <QRandomGenerator>
#include <QWidget>
#include <QUndoGroup>

#include "TabCanvas.h"

class editorInterface;

/**
 * A constructor.
 *
 * This constructor creates a tab with a class diagram.
 *
 * @param p parent widget
 * @param parentGroup pointer to the main undo stack
 * to create a local undo stask
 */
ClassCanvas::ClassCanvas(QWidget *parent, QUndoGroup *parentGroup) : TabCanvas(parent, parentGroup) {
    createScene();
    createEntityClassContextMenu();
    createConnectionContextMenu();
}

/**
 * Generate random [x, y] coordinates ranging from 0 to 600
 *
 * @return coordinates on the scene for new item
 */
QPoint ClassCanvas::generateCoords() const {
    return QPoint(QRandomGenerator::global()->bounded(600), QRandomGenerator::global()->bounded(600));
}

/**
 * // TODO
 * @return list with elements as a pair pointer to the class object
 * and name of this object
 */
QList<QPair<ClassDiagramItem *, QString>> ClassCanvas::getClassStringPairs() {
    QList<QPair<ClassDiagramItem *, QString>> listPairs;
    for (auto x: getItems<ClassDiagramItem>()) {
        QPair<ClassDiagramItem *, QString> pair;
        pair.first = x;
        pair.second = x->_head->toPlainText();
        listPairs.append(pair);
    }
    return listPairs;
}

/**
 * TODO
 * @param buf
 * @return
 */
bool ClassCanvas::createFromFile(dgrm_class_t cls) {
    ItemsBuffer buf;
    for (auto x: cls.classes) {
        buf.addClassItems(x);
    }

    for (auto x: buf.classItems()) {
        ClassDiagramItem *diagramItem = new ClassDiagramItem(x);
        editorScene->addItem(diagramItem);
        diagramItem->setPos(x->x(), x->y());
        editorScene->update();
    }

    QList<ClassDiagramItem *> items = getItems<ClassDiagramItem>();
    for (auto x: cls.concts) {
        buf.addRelationItems(x);
    }

    ClassDiagramItem *from;
    ClassDiagramItem *to;
    for (auto x: buf.relationItems()) {
        for (auto y: items) {
            if (x->leftObj() == y->name()) {
                from = y;
            }
            if (x->rightObj() == y->name()) {
                to = y;
            }
        }

        if (from == nullptr || to == nullptr) {
            return false;
        }

        ClassConnectionItem *item = new ClassConnectionItem(from, to, x,
                                                            static_cast<ClassConnectionItem::ClassConnectionType>(x->type()));
        editorScene->addItem(item);
        editorScene->update();
    }

    return true;
}

/**
 * // TODO
 * @param prg
 */
bool ClassCanvas::getStringRepresentation(Program &prg) {
    std::vector<Class> objects;
    std::vector<Conct> connects;
    ItemsBuffer buf;

    for (auto x: getItems<ClassDiagramItem>()) {
        buf.fillClassItems(x);
    }

    for (auto x: getItems<ClassConnectionItem>()) {
        buf.fillRelationItems(x);
    }

    for (auto x: buf.classItems()) {
        Class tmp;
        tmp.name = x->name().toStdString();
        tmp.width = x->width();
        tmp.height = x->height();
        x->fillColor(tmp.color);
        x->fillCoords(tmp.coords);

        if (!x->splitString(tmp.attrs, x->attrs())) {
            qDebug() << "Error with attribute, color it by red color";
            return false;
        }

        if (!x->splitString(tmp.methods, x->methods())) {
            qDebug() << "Error with method, color it by red color";
            return false;
        }

        prg.diagram_class.classes.push_back(tmp);
    }

    for (auto x: buf.relationItems()) {
        Conct tmp;
        qDebug() << x->rightNum();
        qDebug() << x->leftNum();
        tmp.left_obj = x->leftObj().toStdString();
        tmp.left_num = x->leftNum().toStdString();
        tmp.right_obj = x->rightObj().toStdString();
        tmp.right_num = x->rightNum().toStdString();
        tmp.msg = x->msg().toStdString();
        tmp.arrow = x->type();

        prg.diagram_class.concts.push_back(tmp);
    }

    return true;
}

/**
 * Create and connect all signals for interaction with clasees.
 */
void ClassCanvas::createEntityClassContextMenu() {
    ADD_SIGNAL(addMethod, "Add &Method", "+", "+", this, SLOT(addMethod_triggered()));
    ADD_SIGNAL(rmMethod, "Delete &Method", "+", "+", this, SLOT(rmMethod_triggered()));
    ADD_SIGNAL(addAttr, "Add &Attribute", "+", "+", this, SLOT(addAttr_triggered()));
    ADD_SIGNAL(rmAttr, "Delete &Attribute", "+", "+", this, SLOT(rmAttr_triggered()));

    classMenu = new QMenu();
    classMenu->addAction(addMethod);
    classMenu->addAction(rmMethod);
    classMenu->addAction(addAttr);
    classMenu->addAction(rmAttr);
}

/**
 * Create and connect all signals for interaction with relation arrows.
 */
void ClassCanvas::createConnectionContextMenu() {
    ADD_SIGNAL(aggregation, "Aggregation &Relation", "+", "+", this, SLOT(aggregation_triggered()));
    ADD_SIGNAL(composition, "Composition &Relation", "+", "+", this, SLOT(composition_triggered()));
    ADD_SIGNAL(generalization, "Generalization &Relation", "+", "+", this, SLOT(generalization_triggered()));
    ADD_SIGNAL(association, "Association &Relation", "+", "+", this, SLOT(association_triggered()));
    ADD_SIGNAL(dependency, "Dependency &Relation", "+", "+", this, SLOT(dependency_triggered()));
    ADD_SIGNAL(orientation, "Change &orientation", "+", "+", this, SLOT(orientation_triggered()));

    connectionMenu = new QMenu();
    connectionMenu->addAction(aggregation);
    connectionMenu->addAction(composition);
    connectionMenu->addAction(generalization);
    connectionMenu->addAction(association);
    connectionMenu->addAction(dependency);
    connectionMenu->addAction(orientation);
}

/**
 * Show a context menu with actions for objects.
 *
 * @param pos position on the scene where the click was handled
 */
void ClassCanvas::showContextMenu(const QPoint &pos) {
    QList<QGraphicsItem *> a = editorScene->selectedItems();
    if (a.size() != 1) {
        return;
    }

    if (dynamic_cast<ClassDiagramItem *>(a.first()) != nullptr) {
        classMenu->exec(this->mapToGlobal(pos));
    } else {
        connectionMenu->exec(this->mapToGlobal(pos));
    }
}

/**
 * Add a new method to the selected class at the end.
 */
void ClassCanvas::addMethod_triggered() {
    auto *item = selectedObject<ClassDiagramItem>();

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    // resize item for one row
    item->setRect(0, 0, item->width(), item->height() + item->rowHeight());

    auto line = item->createLine(0, item->height());
    item->pushMethodLine(line);

    auto *text = new ClassTextAttr(item, "+ int example()", QPointF(item->tabText(), item->height() + item->tabText()),
                                   item->flags());
    item->pushMethod(text);

    item->setHeight(item->height() + item->rowHeight());
    for (auto x: item->connections()) {
        x->trackNodes();
    }
};

/**
 * Delete the last method from the selected class.
 */
void ClassCanvas::rmMethod_triggered() {
    auto *item = selectedObject<ClassDiagramItem>();

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
    item->setRect(0, 0, item->width(), item->height() - item->rowHeight());

    // delete line and text
    item->popMethod();
    item->popMethodsLine();

    item->setHeight(item->height() - item->rowHeight());
    for (auto x: item->connections()) {
        x->trackNodes();
    }
};

/**
 * Add a new attribute to the selected class at the end.
 */
void ClassCanvas::addAttr_triggered() {
    auto *item = selectedObject<ClassDiagramItem>();
    if (item == nullptr) {
        qDebug() << "No selected items";
        return;
    }
    // resize item for one row
    item->setRect(0, 0, item->width(), item->height() + item->rowHeight());

    long long inc = item->attrs().size() + 1;
    item->moveTexts(1, inc);
    item->moveLines(1, inc);

    auto line = item->createLine(0, inc * item->rowHeight());
    item->pushAttrLine(line);

    //auto text = item->createText(item->tabText(), item->rowHeight() * inc + item->tabText(), "+ int word");
    ClassTextAttr *text = new ClassTextAttr(item, "+ int word", QPointF(item->tabText(),
                                                                        item->rowHeight() * inc + item->tabText()),
                                            item->flags());
    item->pushAttr(text);
    item->setHeight(item->height() + item->rowHeight());
    for (auto x: item->connections()) {
        x->trackNodes();
    }
};

/**
 * Delete the last method from the selected class.
 */
void ClassCanvas::rmAttr_triggered() {
    ClassDiagramItem *item = selectedObject<ClassDiagramItem>();
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
    item->setRect(0, 0, item->width(), item->height() - item->rowHeight());
    // delete line and text
    item->popAttr();
    item->popAttrsLine();
    // move items
    long long inc = item->attrs().size() + 1;
    item->moveTexts(0, inc);
    item->moveLines(0, inc);
    //item->setPos(tmp_x, tmp_y);
    item->setHeight(item->height() - item->rowHeight());
    for (auto x: item->connections()) {
        x->trackNodes();
    }
};

/**
 * Create an aggregation relation between two selected classes.
 */
void ClassCanvas::aggregation_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Aggregation);
    editorScene->update();
}

/**
 * Create a composition relation between two selected classes.
 */
void ClassCanvas::composition_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Composition);
    editorScene->update();
}

/**
 * Create a generalization relation between two selected classes.
 */
void ClassCanvas::generalization_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Generalization);
    editorScene->update();
}

void ClassCanvas::association_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Association);
    editorScene->update();
}

/**
 * Create an dependency relation between two selected classes.
 */
void ClassCanvas::dependency_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Dependency);
    editorScene->update();
}

/**
 * Change the orientation of the arrow.
 */
void ClassCanvas::orientation_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->changeOrientation();
    editorScene->update();
}

/**
 * Add a new entity to the class diagram.
 */
void ClassCanvas::addEntity() {
    QList<QString> attrs;
    QList<QString> methods;
    QPoint point = generateCoords();

    attrs.push_back("+ int name");
    methods.push_back("+ int name()");

    createItem = new classParams(point.x(), point.y(), "NAME",
                                 color(), 120.0, 120.0,
                                 attrs, methods);
    _undoStack->push(
            new AddClassCommand(editorScene, createItem)
    );
    delete createItem;
}

/**
 * // TODO
 */
void ClassCanvas::addConnection() {
    auto nodes = getSelectedDiagramItems<ClassDiagramItem>();
    auto emptySelect = nodes == QPair<ClassDiagramItem *, ClassDiagramItem *>();
    if (emptySelect) {
        return;
    }

    createRelation = new relationsParams(nodes.first->name(), "1..n", nodes.second->name(),
                                         "0..n", "MSG", ClassConnectionItem::Dependency);
    _undoStack->push(
            new AddClassConnectionCommand(nodes.first, nodes.second, createRelation, ClassConnectionItem::Dependency,
                                          editorScene)
    );
}

/**
 * Paste all selected items from the local "copy" buffer to the scene.
 */
void ClassCanvas::paste() {
    for (auto ptr: buffer->classItems()) {
        auto *diagramItem = new ClassDiagramItem(ptr);
        diagramItem->setPos(ptr->x(), ptr->y());
        editorScene->addItem(diagramItem);
    }
    editorScene->update();
}

/**
 * Copy all selected items to the local "copy" buffer from the scene.
 */
void ClassCanvas::copy() {
    ClassDiagramItem *ptr;
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    buffer->clearBuffer();
    for (auto val: items) {
        ptr = dynamic_cast<ClassDiagramItem *>(val);
        if (ptr != nullptr) {
            buffer->fillClassItems(ptr);
        }
    }
}

/**
 * Copy all selected items to the local "copy" buffer from the scene
 * and the delete them.
 */
void ClassCanvas::cut() {
    copy();
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    for (auto val: items) {
        editorScene->removeItem(val);
    }
}

/**
 * Decrease Z value of the selected items and send them to back.
 */
void ClassCanvas::toBack() {
    _toZchange<ClassCanvas>(false);
}

/**
 * Increase Z value of the selected items and send them to front.
 */
void ClassCanvas::toFront() {
    _toZchange<ClassCanvas>(true);
}

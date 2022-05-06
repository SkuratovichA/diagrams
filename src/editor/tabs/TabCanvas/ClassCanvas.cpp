#include <QGraphicsView>
#include <QUndoStack>
#include <QColor>
#include <QRandomGenerator>
#include <QWidget>
#include <QUndoGroup>

#include "TabCanvas.h"
#include "../diagram/Commands.h"

class editorInterface;

ClassCanvas::ClassCanvas(QWidget *parent, QUndoGroup *parentGroup) : TabCanvas(parent, parentGroup) {
    createScene();
    createEntityClassContextMenu();
    createConnectionContextMenu();
}

QPoint ClassCanvas::generateCoords() const {
    return QPoint(QRandomGenerator::global()->bounded(600), QRandomGenerator::global()->bounded(600));
}

void ClassCanvas::getStringRepresentation(Program &prg) {
    return;
}

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

void ClassCanvas::showContextMenu(const QPoint &pos) {
    auto *item = selectedObject<ClassDiagramItem>();

    if (item == nullptr) {
        return;
    }

    if (dynamic_cast<ClassDiagramItem *>(item) != nullptr) {
        classMenu->exec(this->mapToGlobal(pos));
    } else {
        connectionMenu->exec(this->mapToGlobal(pos));
    }
}

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

    //auto text = item->createText(item->tabText(), item->height() + item->tabText(), "+ int example()");
    auto *text = new ClassTextAttr(item, "+ int example()", item->tabText(), item->height() + item->tabText(),
                                   item->flags());
    item->pushMethod(text);

    item->setHeight(item->height() + item->rowHeight());
    for (auto x: item->connections()) {
        x->trackNodes();
    }
};

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
    ClassTextAttr *text = new ClassTextAttr(item, "+ int word", item->tabText(),
                                            item->rowHeight() * inc + item->tabText(), item->flags());
    item->pushAttr(text);
    item->setHeight(item->height() + item->rowHeight());
    for (auto x: item->connections()) {
        x->trackNodes();
    }
};

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

void ClassCanvas::aggregation_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Aggregation);
    editorScene->update();
}

void ClassCanvas::composition_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Composition);
    editorScene->update();
}

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

void ClassCanvas::dependency_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Dependency);
    editorScene->update();
}

void ClassCanvas::orientation_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->changeOrientation();
    editorScene->update();
}

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

void ClassCanvas::addConnection() {
    auto nodes = getSelectedDiagramItems<ClassDiagramItem>();
    auto emptySelect = nodes == QPair<ClassDiagramItem *, ClassDiagramItem *>();
    if (emptySelect) {
        return;
    }
    _undoStack->push(
            new AddClassConnectionCommand(nodes.first, nodes.second, ClassConnectionItem::Dependency, editorScene)
    );
}

void ClassCanvas::paste() {
    for (auto ptr: buffer->classItems()) {
        auto *diagramItem = new ClassDiagramItem(ptr);
        diagramItem->setPos(ptr->x(), ptr->y());
        editorScene->addItem(diagramItem);
    }
    editorScene->update();
}

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

void ClassCanvas::cut() {
    copy();
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    for (auto val: items) {
        editorScene->removeItem(val);
    }
}
//
//void ClassCanvas::toBack()  {
//    QList<QGraphicsItem *> selItems = editorScene->selectedItems();
//    QList<QGraphicsItem *> allItems = editorScene->items();
//    setZvalue<ClassCanvas>(selItems, 1);
//    for (auto x: selItems) {
//        allItems.removeOne(x);
//    }
//    setZvalue<ClassCanvas *>(allItems, -1);
//}
//
//void ClassCanvas::toFront()  {
//    QList<QGraphicsItem *> selItems = editorScene->selectedItems();
//    QList<QGraphicsItem *> allItems = editorScene->items();
//    setZvalue<ClassCanvas>(selItems, -1);
//    for (auto x: selItems) {
//        allItems.removeOne(x);
//    }
//    setZvalue<ClassCanvas>(allItems, 1);
//}

void ClassCanvas::toBack() {
    _toZchange<ClassCanvas>(false);
}

void ClassCanvas::toFront() {
    _toZchange<ClassCanvas>(true);
}

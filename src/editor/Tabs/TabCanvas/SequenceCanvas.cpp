// File: SequenceCanvas.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include <QGraphicsView>
#include <QUndoStack>
#include <QColor>
#include <QRandomGenerator>
#include <QWidget>
#include <QUndoGroup>

#include "TabCanvas.h"
#include "../../EditorInterface/editorinterface.h"
#include "SequenceConnectionDialog/sequenceconnectiondialog.h"

/**
 * A constructor.
 *
 * This constructor creates a tab with a sequence diagram.
 *
 * @param p parent widget
 * @param parentGroup pointer to the main undo stack
 * to create a local undo stask
 */
SequenceCanvas::SequenceCanvas(QWidget *parent, QUndoGroup *parentGroup) : TabCanvas(parent, parentGroup) {
    parentInterface = dynamic_cast<editorInterface *>(parent);
    createScene();
    createSequenceContextMenu();
}

/**
 * Generate random [x, y] coordinates ranging from 0 to 1200 with a margin
 *
 * @return coordinates on the scene for new item
 */
QPoint SequenceCanvas::generateCoords() const {
    auto x = QRandomGenerator::global()->bounded(600) + 20;
    auto y = 100;
    return QPoint(x, y);
}

/**
 * TODO
 * @param buf
 * @return
 */
bool SequenceCanvas::createFromFile(dgrmSeq_t seq) {
    ItemsBuffer buf;

    for (auto x : seq.actors) {
        buf.addActorItems(x);
    }

    for (auto x : buf.sequenceItems()) {
        SequenceDiagramItem *diagramItem = new SequenceDiagramItem(x);
        editorScene->addItem(diagramItem);
        diagramItem->setPos(x->x(), x->y());
        editorScene->update();
    }

    QList<SequenceDiagramItem *>items = getItems<SequenceDiagramItem>();
    for (auto x : seq.actions) {
        buf.addMessageItems(x);
    }

    SequenceDiagramItem *from;
    SequenceDiagramItem *to;
    for (auto x: buf.messageItems()) {
        for (auto y: items) {
            if (x->nameFrom() == y->name()) {
                from = y;
            }
            if (x->nameTo() == y->name()) {
                to = y;
            }
        }

        if (from == nullptr || to == nullptr) {
            return false;
        }

        SequenceConnectionItem *item = new SequenceConnectionItem(from, to, x,
                              static_cast<Connections::ConnectionType>(x->type()));
        editorScene->addItem(item);
        editorScene->update();
    }


    return true;
}

/**
 * // TODO
 * @param prg
 */
bool SequenceCanvas::getStringRepresentation(Program &prg) {
    //dgrm_seq actors;
    std::vector<Action> actions;
    ItemsBuffer buf;

    dgrmSeq_t obj;

    for (auto x : getItems<SequenceDiagramItem>()) {
        buf.fillActorItems(x);
    }

    for (auto x : getItems<SequenceConnectionItem>()) {
        buf.fillMessageItems(x);
    }

    for (auto x : buf.sequenceItems()) {
        Actor act;
        act.name = x->name().toStdString();
        x->fillColor(act.color);
        x->fillCoords(act.coords);

        obj.actors.push_back(act);
    }

    for (auto x : buf.messageItems()) {
        Action action;
        action.msg = x->msg().toStdString();
        x->fillCoords(action.coords);
        action.from = x->nameFrom().toStdString();
        action.to = x->nameTo().toStdString();
        action.type = x->type();

        obj.actions.push_back(action);
    }

    prg.diagramSequence.push_back(obj);

    // add for sequence diagram
    return true;
}

/**
 * Create and connect all signals for interaction with objects.
 */
void SequenceCanvas::createSequenceContextMenu() {
    ADD_SIGNAL(asynchronousMessage, "Asynchronous Message", "+", "+", this, SLOT(asynchronousMessage_triggered()));
    ADD_SIGNAL(synchronousMessage, "Synchronous Message", "+", "+", this, SLOT(synchronousMessage_triggered()));
    ADD_SIGNAL(returnMessage, "Return Message", "+", "+", this, SLOT(returnMessage_triggered()));
    ADD_SIGNAL(createMessage, "Create Message", "+", "+", this, SLOT(createMessage_triggered()));
    ADD_SIGNAL(deleteMessage, "Delete Message", "+", "+", this, SLOT(deleteMessage_triggered()));

    sequenceMenu = new QMenu();
    sequenceMenu->addAction(asynchronousMessage);
    sequenceMenu->addAction(synchronousMessage);
    sequenceMenu->addAction(returnMessage);
    sequenceMenu->addAction(createMessage);
    sequenceMenu->addAction(deleteMessage);
}

/**
 * Show a context menu with actions for objects.
 *
 * @param pos position on the scene where the click was handled
 */
void SequenceCanvas::showContextMenu(const QPoint &pos) {
    auto *item = selectedObject<SequenceDiagramItem>();

    if (item == nullptr) {
        return;
    }

    if (dynamic_cast<SequenceDiagramItem *>(item) != nullptr) {
        sequenceMenu->exec(this->mapToGlobal(pos));
    } else {
        assert(!"implement sequence menu invocation");
    }
}

/**
 * Create an asynchronous message between two objects.
 */
void SequenceCanvas::asynchronousMessage_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Aggregation);
    editorScene->update();
}

/**
 * Create a synchronous message between two objects.
 */
void SequenceCanvas::synchronousMessage_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Composition);
    editorScene->update();
}

/**
 * Create a return message between two objects.
 */
void SequenceCanvas::returnMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Generalization);
    editorScene->update();
}

/**
 * Create a message for creating a new object.
 */
void SequenceCanvas::createMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Association);
    editorScene->update();
}

/**
 * Create a message for deleting an object.
 */
void SequenceCanvas::deleteMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Dependency);
    editorScene->update();
}

/**
 * Add a new entity to the sequence diagram with the same name as the parent item.
 *
 * @param classDiagramItemParent pointer to an object from a class diagram scene
 */
void SequenceCanvas::addEntity(ClassDiagramItem *classDiagramItemParent) {
    QPoint point = generateCoords();

    createActor = new actorParams(point.x(), point.y(), classDiagramItemParent->name(),
                                  classDiagramItemParent->color());

    _undoStack->push(
            new AddSequenceCommand(editorScene, createActor, classDiagramItemParent)
    );

    delete createActor;
}

/**
 * Add a connection between actor life lines.
 */
void SequenceCanvas::addConnection() {
    auto scd = SequenceConnectionDialog(this);
    scd.exec();

    auto nodes = getSelectedDiagramItems<SequenceDiagramItem>();

    auto emptySelect =
            (nodes == QPair<SequenceDiagramItem *, SequenceDiagramItem *>())
            || nodes.first == nodes.second;
    if (emptySelect) {
        return;
    }
    if (nodes.first == nullptr || nodes.second == nullptr) {
        return;
    }
    auto index = scd.messageType();
    if (index == ConnectionType::Undefined) {
        return;
    }

    paramsMessage = new messageParams(0, 200, "TEXT", nodes.first->name(), nodes.second->name(), index);

    _undoStack->push(
            new AddSequenceConnectionCommand(nodes.first, nodes.second, paramsMessage, index, editorScene)
    );

    delete paramsMessage;
}

/**
 * Paste all selected items from the local "copy" buffer to the scene.
 */
void SequenceCanvas::paste() {
    for (auto ptr: buffer->sequenceItems()) {
        auto *diagramItem = new SequenceDiagramItem(ptr);
        diagramItem->setPos(ptr->x(), ptr->y());
        editorScene->addItem(diagramItem);
    }
    editorScene->update();
}

/**
 * Copy all selected items to the local "copy" buffer from the scene.
 */
void SequenceCanvas::copy() {
    SequenceDiagramItem *ptr;
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    buffer->clearBuffer();
    for (auto val: items) {
        ptr = dynamic_cast<SequenceDiagramItem *>(val);
        if (ptr != nullptr) {
            buffer->fillActorItems(ptr);
        }
    }
}

/**
 * Copy all selected items to the local "copy" buffer from the scene
 * and the delete them.
 */
void SequenceCanvas::cut() {
    copy();
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    for (auto val: items) {
        editorScene->removeItem(val);
    }
}

/**
 * Decrease Z value of the selected items and send them to back.
 */
void SequenceCanvas::toBack() {
    _toZchange<SequenceCanvas>(false);
}

/**
 * Increase Z value of the selected items and send them to front.
 */
void SequenceCanvas::toFront() {
    _toZchange<SequenceCanvas>(true);
}

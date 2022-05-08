// File: SequenceCanvas.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 06.05.2022

#include <QGraphicsView>
#include <QUndoStack>
#include <QColor>
#include <QRandomGenerator>
#include <QWidget>
#include <QUndoGroup>

#include "TabCanvas.h"
#include "../../EditorInterface/editorinterface.h"

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
 * Generate random [x, y] coordinates ranging from 0 to 600
 *
 * @return coordinates on the scene for new item
 */
QPoint SequenceCanvas::generateCoords() const {
    return QPoint(QRandomGenerator::global()->bounded(600), QRandomGenerator::global()->bounded(600));
}

/**
 * // TODO
 * @param prg
 */
bool SequenceCanvas::getStringRepresentation(Program &prg) {
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
    qDebug() << "got name: in constructor " << classDiagramItemParent->name();
    QPoint point = generateCoords();

    createActor = new actorParams(point.x(), point.y(), classDiagramItemParent->name(),
                                  classDiagramItemParent->color(), 80, 50);

    _undoStack->push(
            new AddActorCommand(editorScene, createActor, classDiagramItemParent)
    );

    delete createActor;
}

/**
 * // TODO
 */
void SequenceCanvas::addConnection() {
    assert(!"create new sequence conection command");
//    auto nodes = getSelectedDiagramItems<SequenceDiagramItem>();
//    auto emptySelect = nodes == QPair<SequenceDiagramItem *, SequenceDiagramItem *>();
//    if (emptySelect) {
//        return;
//    }
//    _undoStack->push(
//            new AddActorConnectionCommand(nodes.first, nodes.second, editorScene)
//    );
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

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

//class editorInterface;

SequenceCanvas::SequenceCanvas(QWidget *parent, QUndoGroup *parentGroup) : TabCanvas(parent, parentGroup) {
    parentInterface = dynamic_cast<editorInterface *>(parent);
    createScene();
    createSequenceContextMenu();
}

QPoint SequenceCanvas::generateCoords() const {
    return QPoint(QRandomGenerator::global()->bounded(600), QRandomGenerator::global()->bounded(600));
}

void SequenceCanvas::getStringRepresentation(Program &prg) {
    return;
}

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

void SequenceCanvas::asynchronousMessage_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Aggregation);
    editorScene->update();
}

void SequenceCanvas::synchronousMessage_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Composition);
    editorScene->update();
}

void SequenceCanvas::returnMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Generalization);
    editorScene->update();
}

void SequenceCanvas::createMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Association);
    editorScene->update();
}

void SequenceCanvas::deleteMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Dependency);
    editorScene->update();
}

void SequenceCanvas::addEntity(ClassDiagramItem *classDiagramItemParent) {
    qDebug() << __FILE__ << "name: " << classDiagramItemParent->name();
    QPoint point = generateCoords();
    className = "hello " + className;

    createActor = new actorParams(point.x(), point.y(), classDiagramItemParent->name(),
                                  classDiagramItemParent->color(), 80, 50,
                                  );

    _undoStack->push(
            new AddActorCommand(editorScene, createActor)
    );

    //qDebug() << dynamic_cast<SequenceDiagramItem *>(editorScene->items().last())->_head->toPlainText();

    // TODO 2: probably dont delete this, because there will be a need to remember a pointer or something...
    // TODO 3: just get a pointer to the SequenceDiagramItem to know there it located.
    // TODO 4: probably, there will be a need to do something with deletioen of a sequence diagram?
    // TODO 4; maybe just set a pointer to nullptr, and then check in caller.
    delete createActor;

    return dynamic_cast<SequenceDiagramItem *>(editorScene->items().last());
    //return something; // TODO 1 return a pointier points to the actor
}

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

void SequenceCanvas::paste() {
    for (auto ptr: buffer->sequenceItems()) {
        auto *diagramItem = new SequenceDiagramItem(ptr);
        diagramItem->setPos(ptr->x(), ptr->y());
        editorScene->addItem(diagramItem);
    }
    editorScene->update();
}

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

void SequenceCanvas::cut() {
    copy();
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    for (auto val: items) {
        editorScene->removeItem(val);
    }
}

void SequenceCanvas::toBack() {
    _toZchange<SequenceCanvas>(false);
}

void SequenceCanvas::toFront() {
    _toZchange<SequenceCanvas>(true);
}
// File: Commands.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022

#include "Commands.h"
#include "DiagramItem.h"

#include <QGraphicsScene>

/**
 *
 * @param diagramItem
 * @param oldPos
 * @param parent
 */
MoveCommand::MoveCommand(ActorDiagramItem *diagramItem, const QPointF &oldPos,
                         QUndoCommand *parent)
        : QUndoCommand(parent), diagramItem(diagramItem), startPos(oldPos), newPos(diagramItem->pos()) {
}

/**
 *
 * @param command
 * @return
 */
bool MoveCommand::mergeWith(const QUndoCommand *command) {
    const auto *moveCommand = dynamic_cast<const MoveCommand *>(command);
    ActorDiagramItem *item = moveCommand->diagramItem;
    if (diagramItem != item) {
        return false;
    }
    newPos = item->pos();
    return true;
}

/**
 *
 */
void MoveCommand::undo() {
    diagramItem->setPos(startPos);
    diagramItem->scene()->update();
}

/**
 *
 */
void MoveCommand::redo() {
    diagramItem->setPos(newPos);
}

/**
 *
 * @param scene
 * @param parent
 */
DeleteCommand::DeleteCommand(QGraphicsScene *scene, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    QList<QGraphicsItem *> list = graphicsScene->selectedItems();
    list.first()->setSelected(false);
    diagramItem = dynamic_cast<ActorDiagramItem *>(list.first());
}

/**
 *
 */
void DeleteCommand::undo() {
    graphicsScene->addItem(diagramItem);
    graphicsScene->update();
}

/**
 *
 */
void DeleteCommand::redo() {
    graphicsScene->removeItem(diagramItem);
}

/**
 *
 */
AddActorCommand::AddActorCommand(QGraphicsScene *scene, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    diagramItem = new ActorDiagramItem();
    initialPosition = QPointF((itemCount * 15) % int(scene->width()),
                              (itemCount * 15) % int(scene->height()));
    itemCount++;
    scene->update();
}

/**
 *
 */
AddActorCommand::~AddActorCommand() {
    if (!diagramItem->scene()) {
        delete diagramItem;
    }
}

/**
 *
 */
void AddActorCommand::undo() {
    graphicsScene->removeItem(diagramItem);
    graphicsScene->update();
}

/**
 *
 */
void AddActorCommand::redo() {
    graphicsScene->addItem(diagramItem);
    diagramItem->setPos(initialPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}

/**
 *
 */
AddClassCommand::AddClassCommand(QGraphicsScene *scene, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    diagramItem = new ClassDiagramItem();
    initialPosition = QPointF(((itemCount * 20) + 100)% int(scene->width()),
                              ((itemCount * 20) + 100)% int(scene->height()));
    itemCount++;
    scene->update();
}

/**
 *
 */
AddClassCommand::~AddClassCommand() {
    if (!diagramItem->scene()) {
        delete diagramItem;
    }
}

/**
 *
 */
void AddClassCommand::undo() {
    graphicsScene->removeItem(diagramItem);
    graphicsScene->update();
}

/**
 *
 */
void AddClassCommand::redo() {
    graphicsScene->addItem(diagramItem);
    diagramItem->setPos(initialPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}
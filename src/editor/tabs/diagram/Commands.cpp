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
MoveCommand::MoveCommand(DiagramItem *diagramItem, const QPointF &oldPos,
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
    DiagramItem *item = moveCommand->diagramItem;
    if (diagramItem != item) {
        return false;
    }
    newPos = item->pos();
    setText(QObject::tr("Move %1").arg(createCommandString(diagramItem, newPos)));
    return true;
}

/**
 *
 */
void MoveCommand::undo() {
    diagramItem->setPos(startPos);
    diagramItem->scene()->update();
    setText(QObject::tr("Move %1")
                    .arg(createCommandString(diagramItem, newPos)));
}

/**
 *
 */
void MoveCommand::redo() {
    diagramItem->setPos(newPos);
    setText(QObject::tr("Move %1")
                    .arg(createCommandString(diagramItem, newPos)));
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
    diagramItem = dynamic_cast<DiagramItem *>(list.first());
    setText(QObject::tr("Delete %1")
                    .arg(createCommandString(diagramItem, diagramItem->pos())));
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

    diagramItem = new DiagramItem(DiagramItem::Actor);
    initialPosition = QPointF((itemCount * 15) % int(scene->width()),
                              (itemCount * 15) % int(scene->height()));
//    setText(QObject::tr("Add %1")
//                    .arg(createCommandString(diagramItem, initialPosition)));
    itemCount++;
    scene->update();
    //graphicsScene->addItem(diagramItem);
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

    type = DiagramItem::Class;

    diagramItem = new DiagramItem(type);
    initialPosition = QPointF((itemCount * 15) % int(scene->width()),
                              (itemCount * 15) % int(scene->height()));
//    setText(QObject::tr("Add %1")
//                    .arg(createCommandString(diagramItem, initialPosition)));
    itemCount++;
    scene->update();
    //graphicsScene->addItem(diagramItem);
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


/**
 *
 * @param item
 * @param pos
 * @return
 */
QString createCommandString(DiagramItem *item, const QPointF &pos) {
    static const char *type[5] = {"Actor", "Class", "Message", "Connection", "Unknown"};
    return QObject::tr("%1 at (%3, %4)")
            .arg(type[item->getDiagramType()])
            .arg(pos.x()).arg(pos.y());
}

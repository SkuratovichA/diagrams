// File: Commands.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#include "Commands.h"
#include "Connections.h"
#include "DiagramItem.h"

#include <QGraphicsScene>
#include <utility>

QString createCommandString(QGraphicsItem *item, const QPointF &pos)
{
    return QObject::tr("%1 at (%2, %3)")
            .arg("ITEM")
            .arg(pos.x()).arg(pos.y());
}

/**
 *
 * @param diagramItem
 * @param oldPos
 * @param parent
 */
MoveCommand::MoveCommand(QGraphicsItem *diagramItem, const QPointF &oldPos,
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
    QGraphicsItem *item = moveCommand->diagramItem;
    if (diagramItem != item) {
        return false;
    }
    newPos = item->pos();
    setText(QObject::tr("Move %1")
                    .arg(createCommandString(diagramItem, newPos)));
    return true;
}

/**
 *
 */
void MoveCommand::undo() {
    diagramItem->setPos(startPos.x(), startPos.y());
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
    qDebug() << "Delete item";
    QList<QGraphicsItem *> list = graphicsScene->selectedItems();
    list.first()->setSelected(false);
    diagramItem = list.first();
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


/************************************* Entities */
/**
 *
 */
AddActorCommand::AddActorCommand(QGraphicsScene *scene, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    diagramItem = new ActorDiagramItem();
    initialPosition = QPointF((itemCount * 15 + 100) % int(scene->width()),
                              (itemCount * 15 + 150) % int(scene->height()));
    itemCount++;
    scene->update();

    setText(QObject::tr("Add %1")
                    .arg(createCommandString(static_cast<QGraphicsItem *>(diagramItem), initialPosition)));
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
    initialPosition = QPointF((itemCount * 15 + 100) % int(scene->width()),
                              (itemCount * 15 + 150) % int(scene->height()));

    itemCount++;
    scene->update();
    setText(QObject::tr("Add %1")
                    .arg(createCommandString(static_cast<QGraphicsItem *>(diagramItem), initialPosition)));
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


/************************ Connections */
// TODO: make connectios work somehow
/**
 *
 */
AddClassConnectionCommand::AddClassConnectionCommand(ClassDiagramItem *fromNode,
                                                     QVector<ClassDiagramItem *> toNodes,
                                                     ClassConnectionItem::ClassConnectionType connectionType,
                                                     QGraphicsScene *scene,
                                                     QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    classConnection = new ClassConnectionItem(fromNode, std::move(toNodes), connectionType);
    initialPosition = QPointF(((itemCount * 20) + 100)% int(scene->width()),
                              ((itemCount * 20) + 100)% int(scene->height()));
    itemCount++;
    scene->update();
}

/**
 *
 */
AddClassConnectionCommand::~AddClassConnectionCommand() {
    if (!classConnection->scene()) {
        delete classConnection;
    }
}

/**
 *
 */
void AddClassConnectionCommand::undo() {
    graphicsScene->removeItem(classConnection);
    graphicsScene->update();
}

/**
 *
 */
void AddClassConnectionCommand::redo() {
    graphicsScene->addItem(classConnection);
    classConnection->setPos(initialPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}

/**
 *
 */
AddActorConnectionCommand::AddActorConnectionCommand(ActorDiagramItem *fromNode,
                                                     ActorDiagramItem *toNode,
                                                     ActorConnectionItem::ActorConnectionType connectionType,
                                                     QGraphicsScene *scene,
                                                     QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    actorConnection = new ActorConnectionItem(fromNode, toNode, connectionType);
    initialPosition = QPointF(((itemCount * 20) + 100)% int(scene->width()),
                              ((itemCount * 20) + 100)% int(scene->height()));
    itemCount++;
    scene->update();
}

/**
 *
 */
AddActorConnectionCommand::~AddActorConnectionCommand() {
    if (!actorConnection->scene()) {
        delete actorConnection;
    }
}

/**
 *
 */
void AddActorConnectionCommand::undo() {
    graphicsScene->removeItem(actorConnection);
    graphicsScene->update();
}

/**
 *
 */
void AddActorConnectionCommand::redo() {
    graphicsScene->addItem(actorConnection);
    actorConnection->setPos(initialPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}
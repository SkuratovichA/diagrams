// File: Commands.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#include "Commands.h"
#include "../Connections/Connections.h"
#include "../DiagramItem.h"

#include <QGraphicsScene>

QString createCommandString(QGraphicsItem *item) {
    return QObject::tr("%1")
            .arg("ITEM");
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
    qDebug() << "Move command";
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
    setText(QObject::tr("Move %1")
                    .arg(createCommandString(diagramItem)));
    return true;
}

/**
 *
 */
void MoveCommand::undo() {
    diagramItem->setPos(startPos.x(), startPos.y());
    diagramItem->scene()->update();
    setText(QObject::tr("Move %1")
                    .arg(createCommandString(diagramItem)));
}

/**
 *
 */
void MoveCommand::redo() {
    diagramItem->setPos(newPos);
    setText(QObject::tr("Move %1")
                    .arg(createCommandString(diagramItem)));
}

/**
 *
 * @param scene
 * @param parent
 */
DeleteCommand::DeleteCommand(QGraphicsScene *scene, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    listItems = graphicsScene->selectedItems();

    for (auto x: listItems) {
        setText(QObject::tr("Delete %1")
                        .arg(createCommandString(x)));

        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
                    foreach (ClassConnectionItem *connection, connections) {
                    scene->removeItem(connection);
                }
        }
    }
}

/**
 *
 */
void DeleteCommand::undo() {

    for (auto x: listItems) {
        graphicsScene->addItem(x);
        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
                    foreach (ClassConnectionItem *connection, connections) {
                    graphicsScene->addItem(connection);
                }
        }
    }
    graphicsScene->update();
}

/**
 *
 */
void DeleteCommand::redo() {

    for (auto x: listItems) {
        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
                    foreach (ClassConnectionItem *connection, connections) {
                    graphicsScene->removeItem(connection);
                }
        }
        graphicsScene->removeItem(x);
    }
}


/************************************* Entities */
/**
 *
 */
AddActorCommand::AddActorCommand(QGraphicsScene *scene, actorParams *params, ClassDiagramItem *parentClassDiagramItem,QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    diagramItem = new SequenceDiagramItem(params, parentClassDiagramItem);
    initialStartPosition = QPointF(params->x(), params->y());
    itemCount++;
    scene->update();
    setText(QObject::tr("Add %1")
                    .arg(createCommandString(static_cast<QGraphicsItem *>(diagramItem))));
}

/**
 *
 */
AddActorCommand::~AddActorCommand() {
    if (diagramItem->scene() == nullptr) {
        return;
    }
    delete diagramItem;
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
    diagramItem->setPos(initialStartPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}

/**
 *
 */
AddClassCommand::AddClassCommand(QGraphicsScene *scene, classParams *params, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    qDebug() << params->name();
    diagramItem = new ClassDiagramItem(params);
    initialStartPosition = QPointF(params->x(), params->y());
    itemCount++;
    scene->update();
    setText(QObject::tr("Add %1")
                    .arg(createCommandString(static_cast<QGraphicsItem *>(diagramItem))));
}

/**
 *
 */
AddClassCommand::~AddClassCommand() {
    if (diagramItem->scene() != nullptr) {
        return;
    }
    delete diagramItem;
    qDebug() << "diagramItem deleted (Commands.cpp)";
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
    diagramItem->setPos(initialStartPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}


/************************ Connections */
// TODO: make connectios work somehow
/**
 *
 */
AddClassConnectionCommand::AddClassConnectionCommand(ClassDiagramItem *fromNode,
                                                     ClassDiagramItem *toNodes,
                                                     ClassConnectionItem::ClassConnectionType connectionType,
                                                     QGraphicsScene *scene,
                                                     QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    auto maxConnectedElements = std::max(fromNode->occupiedSockets(), toNodes->occupiedSockets());
    qDebug() << "max number of occupied sockets: " << maxConnectedElements;

    classConnection = new ClassConnectionItem(fromNode, toNodes, connectionType, maxConnectedElements);
    setText(QObject::tr("Connect %1")
                    .arg(createCommandString(static_cast<ClassConnectionItem *>(classConnection))));

    scene->update();
}

/**
 *
 */
AddClassConnectionCommand::~AddClassConnectionCommand() {
    if (classConnection->scene() != nullptr) {
        return;
    }
    qDebug() << "This shit of code causes segfault";
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
    graphicsScene->clearSelection();
    graphicsScene->update();
}

/**
 *
 */
AddActorConnectionCommand::AddActorConnectionCommand(SequenceDiagramItem *fromNode,
                                                     SequenceDiagramItem *toNode,
                                                     ActorConnectionItem::ActorConnectionType connectionType,
                                                     QGraphicsScene *scene,
                                                     QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    actorConnection = new ActorConnectionItem(fromNode, toNode, connectionType);
    scene->update();
}

/**
 *
 */
AddActorConnectionCommand::~AddActorConnectionCommand() {
    if (actorConnection->scene() != nullptr) {
        return;
    }
    delete actorConnection;
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
    graphicsScene->clearSelection();
    graphicsScene->update();
}
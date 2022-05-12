// File: Commands.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#include "Commands.h"
#include "../Connections/Connections.h"
#include "../DiagramItem/DiagramItem.h"

#include <QGraphicsScene>

using namespace Connections;

/**
 *
 * @param item
 * @return
 */
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
            for (auto connection: connections) {
                scene->removeItem(connection);
            }
        } else if (dynamic_cast<SequenceDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<SequenceDiagramItem *>(x)->connections();
            for (auto connection: connections) {
                auto nodeFrom = connection->nodeFrom();
                auto nodeTo = connection->nodeTo();
                nodeFrom->removeConnection(connection);
                nodeTo->removeConnection(connection);
                scene->removeItem(connection);
            }
        } else if (dynamic_cast<SequenceConnectionItem *>(x) != nullptr) {
            // remove a connection activity boxes from lifeLine
            auto connection = dynamic_cast<SequenceConnectionItem *>(x);
            auto nodeFrom = connection->nodeFrom();
            auto nodeTo = connection->nodeTo();
            nodeFrom->removeConnection(connection);
            nodeTo->removeConnection(connection);
            scene->removeItem(connection);
        }
    }
}

void DeleteCommand::undo() {
    for (auto x: listItems) {
        graphicsScene->addItem(x);
        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
            for (auto connection: connections) {
                graphicsScene->addItem(connection);
            }
        } else if (dynamic_cast<SequenceDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<SequenceDiagramItem *>(x)->connections();
            for (auto connection: connections) {
                auto nodeFrom = connection->nodeFrom();
                auto nodeTo = connection->nodeTo();
                nodeFrom->addConnection(connection, ActorType::Caller);
                nodeTo->addConnection(connection, ActorType::Receiver);
                graphicsScene->addItem(connection);
            }
        } else if (dynamic_cast<SequenceConnectionItem *>(x) != nullptr) {
            // remove a connection activity boxes from lifeLine
            auto connection = dynamic_cast<SequenceConnectionItem *>(x);
            auto nodeFrom = connection->nodeFrom();
            auto nodeTo = connection->nodeTo();
            nodeFrom->addConnection(connection, ActorType::Caller);
            nodeTo->addConnection(connection, ActorType::Receiver);
            graphicsScene->addItem(connection);
        }
    }
    graphicsScene->update();
}

void DeleteCommand::redo() {
    for (auto x: listItems) {
        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
            for (auto connection: connections) {
                graphicsScene->removeItem(connection);
            }
        } else
        if (dynamic_cast<SequenceDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<SequenceDiagramItem *>(x)->connections();
            for (auto connection: connections) {
                auto nodeFrom = connection->nodeFrom();
                auto nodeTo = connection->nodeTo();
                nodeFrom->removeConnection(connection);
                nodeTo->removeConnection(connection);
                graphicsScene->removeItem(connection);
            }
        } else
        if (dynamic_cast<SequenceConnectionItem *>(x) != nullptr) {
            // remove a connection activity boxes from lifeLine
            auto connection = dynamic_cast<SequenceConnectionItem *>(x);
            auto nodeFrom = connection->nodeFrom();
            auto nodeTo = connection->nodeTo();
            nodeFrom->removeConnection(connection);
            nodeTo->removeConnection(connection);
            graphicsScene->removeItem(connection);
        }
        graphicsScene->removeItem(x);
    }
}

/**
 *
 */
AddSequenceCommand::AddSequenceCommand(QGraphicsScene *scene, actorParams *params,
                                       ClassDiagramItem *parentClassDiagramItem, QUndoCommand *parent)
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
AddSequenceCommand::~AddSequenceCommand() {
    if (diagramItem->scene() == nullptr) {
        return;
    }
    delete diagramItem;
}

/**
 *
 */
void AddSequenceCommand::undo() {
    graphicsScene->removeItem(diagramItem);
    graphicsScene->update();
}

/**
 *
 */
void AddSequenceCommand::redo() {
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
AddClassCommand::~AddClassCommand() { }

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

/**
 *
 */
AddClassConnectionCommand::AddClassConnectionCommand(ClassDiagramItem *fromNode,
                                                     ClassDiagramItem *toNodes,
                                                     relationsParams *params,
                                                     ClassConnectionItem::ClassConnectionType type,
                                                     QGraphicsScene *scene,
                                                     QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    auto maxConnectedElements = std::max(fromNode->occupiedSockets(), toNodes->occupiedSockets());

    classConnection = new ClassConnectionItem(fromNode, toNodes, params, type, maxConnectedElements);
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
    delete classConnection;
//    "This shit of code causes segfault";
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
AddSequenceConnectionCommand::AddSequenceConnectionCommand(SequenceDiagramItem *fromNode,
                                                           SequenceDiagramItem *toNode,
                                                           ConnectionType connectionType,
                                                           QGraphicsScene *scene,
                                                           QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {

    actorConnection = new SequenceConnectionItem(fromNode, toNode, connectionType);
    initialStartPosition = QPointF(100, 100);
    setText(QObject::tr("Connect %1")
                    .arg(createCommandString(static_cast<SequenceConnectionItem *>(actorConnection))));
    scene->update();

}

/**
 *
 */
AddSequenceConnectionCommand::~AddSequenceConnectionCommand() {
    if (actorConnection->scene() != nullptr) {
        return;
    }
    delete actorConnection;
}

/**
 *
 */
void AddSequenceConnectionCommand::undo() {
    auto nodeFrom = actorConnection->nodeFrom();
    auto nodeTo = actorConnection->nodeTo();
    nodeFrom->removeConnection(actorConnection);
    nodeTo->removeConnection(actorConnection);
    graphicsScene->removeItem(actorConnection);
    graphicsScene->update();
}

/**
 *
 */
void AddSequenceConnectionCommand::redo() {
    auto nodeFrom = actorConnection->nodeFrom();
    auto nodeTo = actorConnection->nodeTo();
    nodeFrom->addConnection(actorConnection, ActorType::Caller);
    nodeTo->addConnection(actorConnection, ActorType::Receiver);
    graphicsScene->addItem(actorConnection);
    graphicsScene->clearSelection();
    graphicsScene->update();
}
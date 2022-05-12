// File: Commands.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#include "Commands.h"

#include <QGraphicsScene>

using namespace Connections;



MoveCommand::MoveCommand(QGraphicsItem *diagramItem, const QPointF &oldPos,
                         QUndoCommand *parent)
        : QUndoCommand(parent), diagramItem(diagramItem), startPos(oldPos), newPos(diagramItem->pos()) {
}

bool MoveCommand::mergeWith(const QUndoCommand *command) {
    const auto *moveCommand = dynamic_cast<const MoveCommand *>(command);
    QGraphicsItem *item = moveCommand->diagramItem;
    if (diagramItem != item) {
        return false;
    }
    return true;
}

void MoveCommand::undo() {
    diagramItem->setPos(startPos.x(), startPos.y());
    diagramItem->scene()->update();
}

void MoveCommand::redo() {
    diagramItem->setPos(newPos);
}

DeleteCommand::DeleteCommand(QGraphicsScene *scene, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    listItems = graphicsScene->selectedItems();
    for (auto x: listItems) {
        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
            dynamic_cast<ClassDiagramItem *>(x)->setDeleted(true);
            for (auto connection: connections) {
                scene->removeItem(connection);
            }
        } else if (dynamic_cast<SequenceDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<SequenceDiagramItem *>(x)->connections();
            for (auto connection: connections) {
                auto nodeFrom = connection->nodeFrom();
                auto nodeTo = connection->nodeTo();
                nodeFrom->removeConnection(connection, CommandType::Delete, CommandType::Self);
                nodeTo->removeConnection(connection, CommandType::Delete, CommandType::Self);
                scene->removeItem(connection);
            }
        } else if (dynamic_cast<SequenceConnectionItem *>(x) != nullptr) {
            // remove a connection activity boxes from lifeLine
            auto connection = dynamic_cast<SequenceConnectionItem *>(x);
            auto nodeFrom = connection->nodeFrom();
            auto nodeTo = connection->nodeTo();
            nodeFrom->removeConnection(connection, CommandType::Delete, CommandType::Connection);
            nodeTo->removeConnection(connection, CommandType::Delete, CommandType::Connection);
            scene->removeItem(connection);
        }
    }
    scene->update();
}

void DeleteCommand::undo() {
    for (auto x: listItems) {
        graphicsScene->addItem(x);
        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
            dynamic_cast<ClassDiagramItem *>(x)->setDeleted(false);
            for (auto connection: connections) {
                graphicsScene->addItem(connection);
            }
        } else if (dynamic_cast<SequenceDiagramItem *>(x) != nullptr) {
            // TODO:
            auto connections = dynamic_cast<SequenceDiagramItem *>(x)->getRemovedConnectionsOnDeleteSelf();
            for (auto connection: connections) {
                auto nodeFrom = connection->nodeFrom();
                auto nodeTo = connection->nodeTo();
                nodeFrom->addConnection(connection, ActorType::Caller, CommandType::Delete, CommandType::Self);
                nodeTo->addConnection(connection, ActorType::Receiver, CommandType::Delete, CommandType::Self);
                graphicsScene->addItem(connection);
            }
        } else if (dynamic_cast<SequenceConnectionItem *>(x) != nullptr) {
            // remove a connection activity boxes from lifeLine
            auto connection = dynamic_cast<SequenceConnectionItem *>(x);
            auto nodeFrom = connection->nodeFrom();
            auto nodeTo = connection->nodeTo();
            nodeFrom->addConnection(connection, ActorType::Caller, CommandType::Delete, CommandType::Connection);
            nodeTo->addConnection(connection, ActorType::Receiver, CommandType::Delete, CommandType::Connection);
            graphicsScene->addItem(connection);
        }
    }
    graphicsScene->update();
}

void DeleteCommand::redo() {
    for (auto x: listItems) {
        if (dynamic_cast<ClassDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<ClassDiagramItem *>(x)->connections();
            dynamic_cast<ClassDiagramItem *>(x)->setDeleted(true);
            for (auto connection: connections) {
                graphicsScene->removeItem(connection);
            }
        } else
        if (dynamic_cast<SequenceDiagramItem *>(x) != nullptr) {
            auto connections = dynamic_cast<SequenceDiagramItem *>(x)->getRemovedConnectionsOnDeleteSelf();
            for (auto connection: connections) {
                auto nodeFrom = connection->nodeFrom();
                auto nodeTo = connection->nodeTo();
                nodeFrom->removeConnection(connection, CommandType::Delete, CommandType::Self);
                nodeTo->removeConnection(connection, CommandType::Delete, CommandType::Self);
                graphicsScene->removeItem(connection);
            }
        } else
        if (dynamic_cast<SequenceConnectionItem *>(x) != nullptr) {
            // remove a connection activity boxes from lifeLine
            auto connection = dynamic_cast<SequenceConnectionItem *>(x);
            auto nodeFrom = connection->nodeFrom();
            auto nodeTo = connection->nodeTo();
            nodeFrom->removeConnection(connection, CommandType::Delete, CommandType::Connection);
            nodeTo->removeConnection(connection, CommandType::Delete, CommandType::Connection);
            graphicsScene->removeItem(connection);
        }
        graphicsScene->removeItem(x);
    }
    graphicsScene->update();
}

AddSequenceCommand::AddSequenceCommand(QGraphicsScene *scene, SequenceDiagramItemParameters *params,
                                       ClassDiagramItem *parentClassDiagramItem, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    diagramItem = new SequenceDiagramItem(params, parentClassDiagramItem);
    initialStartPosition = QPointF(params->x(), params->y());
    scene->update();
}

AddSequenceCommand::~AddSequenceCommand() {}

void AddSequenceCommand::undo() {
    graphicsScene->removeItem(diagramItem);
    graphicsScene->update();
}

void AddSequenceCommand::redo() {
    graphicsScene->addItem(diagramItem);
    diagramItem->setPos(initialStartPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}

AddClassCommand::AddClassCommand(QGraphicsScene *scene, classParams *params, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    diagramItem = new ClassDiagramItem(params);
    initialStartPosition = QPointF(params->x(), params->y());
    scene->update();
}

AddClassCommand::~AddClassCommand() { }

void AddClassCommand::undo() {
    dynamic_cast<ClassDiagramItem *>(diagramItem)->setDeleted(true);
    graphicsScene->removeItem(diagramItem);
    graphicsScene->update();
}

void AddClassCommand::redo() {
    dynamic_cast<ClassDiagramItem *>(diagramItem)->setDeleted(false);
    graphicsScene->addItem(diagramItem);
    diagramItem->setPos(initialStartPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}

AddClassConnectionCommand::AddClassConnectionCommand(ClassDiagramItem *fromNode,
                                                     ClassDiagramItem *toNode,
                                                     relationsParams *params,
                                                     ClassConnectionItem::ClassConnectionType type,
                                                     QGraphicsScene *scene,
                                                     QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    auto maxConnectedElements = std::max(fromNode->occupiedSockets(), toNode->occupiedSockets());

    classConnection = new ClassConnectionItem(fromNode, toNode, params, type, maxConnectedElements);
    scene->update();
}

AddClassConnectionCommand::~AddClassConnectionCommand() {}

void AddClassConnectionCommand::undo() {
    graphicsScene->removeItem(classConnection);
    graphicsScene->update();
}

void AddClassConnectionCommand::redo() {
    graphicsScene->addItem(classConnection);
    graphicsScene->clearSelection();
    graphicsScene->update();
}

AddSequenceConnectionCommand::AddSequenceConnectionCommand(SequenceDiagramItem *fromNode,
                                                           SequenceDiagramItem *toNode,
                                                           ConnectionType connectionType,
                                                           QGraphicsScene *scene,
                                                           QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {

    actorConnection = new SequenceConnectionItem(fromNode, toNode, connectionType);
    initialStartPosition = QPointF(100, 100);
    scene->update();

}

AddSequenceConnectionCommand::~AddSequenceConnectionCommand() {}

void AddSequenceConnectionCommand::undo() {
    auto nodeFrom = actorConnection->nodeFrom();
    auto nodeTo = actorConnection->nodeTo();
    nodeFrom->removeConnection(actorConnection, CommandType::Add, CommandType::Connection);
    nodeTo->removeConnection(actorConnection, CommandType::Add, CommandType::Connection);
    graphicsScene->removeItem(actorConnection);
    graphicsScene->update();
}

void AddSequenceConnectionCommand::redo() {
    auto nodeFrom = actorConnection->nodeFrom();
    auto nodeTo = actorConnection->nodeTo();
    nodeFrom->addConnection(actorConnection, ActorType::Caller, CommandType::Add, CommandType::Connection);
    nodeTo->addConnection(actorConnection, ActorType::Receiver, CommandType::Add, CommandType::Connection);
    graphicsScene->addItem(actorConnection);
    graphicsScene->clearSelection();
    graphicsScene->update();
}
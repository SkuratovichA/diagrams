// File: Commands.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#include "Commands.h"
#include "Connections.h"
#include "DiagramItem.h"

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
    qDebug() << " iam heeeeeeeeeer";
    QList<QGraphicsItem *> list = graphicsScene->selectedItems();
    list.first()->setSelected(false);
    diagramItem = list.first();
    setText(QObject::tr("Delete %1")
                    .arg(createCommandString(diagramItem)));
    if (dynamic_cast<ClassDiagramItem *>(diagramItem) != nullptr) {
        auto connections = dynamic_cast<ClassDiagramItem *>(diagramItem)->connections();
        foreach (ClassConnectionItem *connection, connections) {

            scene->removeItem(connection);
        }
    }
}

/**
 *
 */
void DeleteCommand::undo() {
    graphicsScene->addItem(diagramItem);
    if (dynamic_cast<ClassDiagramItem *>(diagramItem) != nullptr) {
        auto connections = dynamic_cast<ClassDiagramItem *>(diagramItem)->connections();
                foreach (ClassConnectionItem *connection, connections) {
                graphicsScene->addItem(connection);
            }
    }
    graphicsScene->update();
}

/**
 *
 */
void DeleteCommand::redo() {
    if (dynamic_cast<ClassDiagramItem *>(diagramItem) != nullptr) {
        auto connections = dynamic_cast<ClassDiagramItem *>(diagramItem)->connections();
                foreach (ClassConnectionItem *connection, connections) {
                graphicsScene->removeItem(connection);
            }
    }
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
    initialStartPosition = QPointF(static_cast<qreal>((itemCount * 15 + 100) % static_cast<int>(scene->width())),
                                   static_cast<qreal>((itemCount * 15 + 150) % static_cast<int>(scene->height())));
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
//    static int itemCount = 0;

    classConnection = new ClassConnectionItem(fromNode, toNodes, connectionType);
//    initialStartPosition = QPointF(static_cast<qreal>(((itemCount * 20) + 100) % static_cast<int>(scene->width())),
//                                   static_cast<qreal>(((itemCount * 20) + 100) % static_cast<int>(scene->height())));
//    itemCount++;
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
//    delete classConnection;
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
//    classConnection->setPos(initialStartPosition);
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
//    static int itemCount = 0;

    actorConnection = new ActorConnectionItem(fromNode, toNode, connectionType);
//    initialStartPosition = QPointF(static_cast<qreal>(((itemCount * 20) + 100) % static_cast<int>(scene->width())),
//                                   static_cast<qreal>(((itemCount * 20) + 100) % static_cast<int>(scene->height())));
//    itemCount++;
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
//    actorConnection->setPos(initialStartPosition);
    graphicsScene->clearSelection();
    graphicsScene->update();
}
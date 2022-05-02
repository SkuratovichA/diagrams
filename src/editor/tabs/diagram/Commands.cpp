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
    qDebug() << "A chto tut proisxodit??";
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
AddEntityCommand::AddEntityCommand(DiagramItem::EntityType addType,
                                   QGraphicsScene *scene, QUndoCommand *parent)
        : QUndoCommand(parent), graphicsScene(scene) {
    static int itemCount = 0;

    //diagramItem = new DiagramItem(addType);

    switch (addType) {
        case DiagramItem::Actor:
            qDebug() << "You try to create an actor.";
            diagramItem = new Actor(0,0,1);
            qDebug() << "actor was created" << diagramItem;
            qDebug() << "Scene before push actor " << graphicsScene;
            //graphicsScene->addItem(diagramItem);
            break;
        case DiagramItem::Class:
            qDebug() << "You try to create a class.";
            diagramItem = new Class(0,0,1);

            initialPosition = QPointF((itemCount * 15) % int(scene->width()),
                                      (itemCount * 15) % int(scene->height()));
            scene->update();
            ++itemCount;
            setText(QObject::tr("Add %1")
                            .arg(createCommandString(diagramItem, initialPosition)));
            break;
        case DiagramItem::ActorConnection:
            qDebug() << "You try to create a connection for actors.";
            diagramItem = new Message(0,0,1);
            break;
        case DiagramItem::ClassConnection:
            qDebug() << "You try to create a connection for classes.";
            diagramItem = new Relation(0,0,1);
            break;
        default:
            assert(!"here, DiagramItem type must be specified. Panicking and exiting.");
    }
}

/**
 *
 */
AddEntityCommand::~AddEntityCommand() {
    if (!diagramItem->scene()) {
        delete diagramItem;
    }
}

/**
 *
 */
void AddEntityCommand::undo() {
    graphicsScene->removeItem(diagramItem);
    graphicsScene->update();
}

/**
 *
 */
void AddEntityCommand::redo() {
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
            .arg(type[item->getEntityType()])
            .arg(pos.x()).arg(pos.y());
}

// File: Commands.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#ifndef DIAGRAMS_COMMANDS_H
#define DIAGRAMS_COMMANDS_H

#include <QUndoCommand>
#include "DiagramItem.h"

class MoveCommand : public QUndoCommand {
public:
    enum {
        Id = 1234
    };

    MoveCommand(ActorDiagramItem *diagramItem, const QPointF &startPos,
                QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *command) override;

    int id() const override { return Id; }

private:
    ActorDiagramItem *diagramItem;
    QPointF startPos;
    QPointF newPos;
};

class DeleteCommand : public QUndoCommand {
public:
    explicit DeleteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;

private:
    ActorDiagramItem *diagramItem;
    QGraphicsScene *graphicsScene;
};

class AddActorCommand : public QUndoCommand {
public:
    explicit AddActorCommand(QGraphicsScene *scene, QUndoCommand *parent = nullptr);
    ~AddActorCommand();
    void undo() override;
    void redo() override;


private:
    ActorDiagramItem *diagramItem;
    QGraphicsScene *graphicsScene;
    union {
        QPointF initialStartPosition;
        QPointF initialPosition;
    };
    QPointF initialEndPosition;
};

class AddClassCommand : public QUndoCommand {
public:
    explicit AddClassCommand(QGraphicsScene *scene, QUndoCommand *parent = nullptr);
    ~AddClassCommand();
    void undo() override;
    void redo() override;

private:
    ClassDiagramItem *diagramItem;
    QGraphicsScene *graphicsScene;
    union {
        QPointF initialStartPosition;
        QPointF initialPosition;
    };
    QPointF initialEndPosition;
};

QString createCommandString(ActorDiagramItem *item, const QPointF &point);

#endif //DIAGRAMS_COMMANDS_H
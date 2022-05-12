// File: Commands.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#ifndef DIAGRAMS_COMMANDS_H
#define DIAGRAMS_COMMANDS_H

#include <QUndoCommand>
#include "../Connections/Connections.h"
#include "../DiagramItem/DiagramItem.h"
#include "../FillItems/ObjectParams.h"

using namespace Connections;

/**
 *
*/
class MoveCommand : public QUndoCommand {
public:
    enum {
        Id = 1234
    };

    MoveCommand(QGraphicsItem *diagramItem, const QPointF &startPos,
                QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    bool mergeWith(const QUndoCommand *command) override;

    [[nodiscard]] int id() const override {return Id;}

private:
    QGraphicsItem *diagramItem;
    QPointF startPos;
    QPointF newPos;
};

/**
 *
 */
class DeleteCommand : public QUndoCommand {
public:
    explicit DeleteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem *> listItems;
    QGraphicsScene *graphicsScene;
};

/**
 *
 */
class AddSequenceCommand : public QUndoCommand {
public:
    explicit AddSequenceCommand(QGraphicsScene *scene, SequenceDiagramItemParameters *params, ClassDiagramItem *parentClassDiagramItem,
                                QUndoCommand *parent = nullptr);

    ~AddSequenceCommand();

    void undo() override;
    void redo() override;

private:
    SequenceDiagramItem *diagramItem;
    QGraphicsScene *graphicsScene;
    QPointF initialStartPosition;
    QPointF initialEndPosition;
};

/**
 *
 */
class AddClassCommand : public QUndoCommand {
public:
    explicit AddClassCommand(QGraphicsScene *scene, classParams *params, QUndoCommand *parent = nullptr);

    ~AddClassCommand();

    void undo() override;
    void redo() override;

private:
    ClassDiagramItem *diagramItem;
    QGraphicsScene *graphicsScene;
    QPointF initialStartPosition;
    QPointF initialEndPosition;
};

/**
 *
 */
class AddSequenceConnectionCommand : public QUndoCommand {
public:
    explicit AddSequenceConnectionCommand(SequenceDiagramItem *fromNode,
                                          SequenceDiagramItem *toNode,
                                          messageParams *params,
                                          ConnectionType connectionType,
                                          QGraphicsScene *scene,
                                          QUndoCommand *parent = nullptr);

    ~AddSequenceConnectionCommand();

    void undo() override;
    void redo() override;

private:
    SequenceConnectionItem *actorConnection;
    QGraphicsScene *graphicsScene;
    QPointF initialStartPosition;
//    QPointF initialStartPosition;
//    QPointF initialEndPosition;
};

/**
 *
 */
class AddClassConnectionCommand : public QUndoCommand {
public:
    explicit AddClassConnectionCommand(ClassDiagramItem *fromNode,
                                       ClassDiagramItem *toNode,
                                       relationsParams *params,
                                       ClassConnectionItem::ClassConnectionType type,
                                       QGraphicsScene *scene,
                                       QUndoCommand *parent = nullptr);

    ~AddClassConnectionCommand();

    void undo() override;
    void redo() override;

private:
    ClassConnectionItem *classConnection;
    QGraphicsScene *graphicsScene;
//    QPointF initialStartPosition;
//    QPointF initialEndPosition;
};

QString createCommandString(SequenceDiagramItem *item, const QPointF &point);

#endif //DIAGRAMS_COMMANDS_H

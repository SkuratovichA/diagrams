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
 * Moves an object on the scene with undo/redo commands supported
 */
class MoveCommand : public QUndoCommand {
public:
    /**
     * Enum taken from the example.
     */
    enum {
        Id = 1234
    };

    /**
     * @brief A Constructor.
     * @param diagramItem Object on the diagram scene to perfom a move operation on.
     * @param startPos Initial position of an object
     * @param parent Parent undo stack
     */
    MoveCommand(QGraphicsItem *diagramItem, const QPointF &startPos,
                QUndoCommand *parent = nullptr);

    /**
     * @brief Undo command
     */
    void undo() override;

    /**
     * @brief Redo command
     */
    void redo() override;

    /**
     * @brief Functino for undo/redo commands.
     * @param command
     * @return
     */
    bool mergeWith(const QUndoCommand *command) override;

    /**
     * @brief Getter.
     * @return id
     */
    [[nodiscard]] int id() const override {return Id;}

private:
    QGraphicsItem *diagramItem; ///< diagram item to move
    QPointF startPos; ///< starting position
    QPointF newPos; ///< end position
};

/**
 * A class for supporting deletion of the objects.
 */
class DeleteCommand : public QUndoCommand {
public:
    /**
     * @brief A constructor.
     * @details
     * In case of a ClassDiagramItem, an object is removed from the scene as well as all the connections.
     * In case of a ClassConnectionItem, only a connection is removed from the scene.
     * In case of a SequencenDiagramItem, an object is removed from the scene as well as all of its connections.
     * In case of a SequenceConnectionItem, a connection is removed from the scene.
     * @param graphicsScene A graphics scene to dele the items from
     * @param parent parent undo command
     */
    explicit DeleteCommand(QGraphicsScene *graphicsScene,
                           QUndoCommand *parent = nullptr);

    /**
     * @brief Undo command
     * @details
     * In case of a ClassDiagramItem, an object is placed back to the scene.
     * In case of a ClassConnectionItem, a connection is returned to the scene.
     * In case of a SequencenDiagramItem, an object is placed back to the scene.
     * In case of a SequenceConnectionItem, a connection is returned to the scene.
     */
    void undo() override;

    /**
     * @brief Redo command
     * @details
     * In case of a ClassDiagramItem, an object is removed from the scene as well as all the connections.
     * In case of a ClassConnectionItem, only a connection is removed from the scene.
     * In case of a SequencenDiagramItem, an object is removed from the scene as well as all of its connections.
     * In case of a SequenceConnectionItem, a connection is removed from the scene.
     */
    void redo() override;

private:
    QList<QGraphicsItem *> listItems; ///< list of iems to delete from a grafhics scene
    QGraphicsScene *graphicsScene; ///< a graphics scene to delete the items from.
};

/**
 * Command for adding a sequence command with undo/redo commands supported
 */
class AddSequenceCommand : public QUndoCommand {
public:
    /**
     * @brief A constructor.
     * @param scene Graphics scene
     * @param params Parameters of an actor (sequence diagram object)
     * @param parentClassDiagramItem Class diagram item actor created form.
     * @param parent parent undo command.
     */
    explicit AddSequenceCommand(QGraphicsScene *scene,
                                SequenceDiagramItemParameters *params,
                                ClassDiagramItem *parentClassDiagramItem,
                                QUndoCommand *parent = nullptr);

    /**
     * @brief A destructor.
     */
    ~AddSequenceCommand();

    /**
     * @brief Undo command
     */
    void undo() override;

    /**
     * @brief Redo command
     */
    void redo() override;

private:
    SequenceDiagramItem *diagramItem; ///< Sequence diagram item object (actor with a timeline)
    QGraphicsScene *graphicsScene; ///< Graphics scene to add an actor to.
    QPointF initialStartPosition; ///< position where newly created actor will be placed
};

/**
 * Command for adding a class to the a graphics scene with undo/redo commands support.
 */
class AddClassCommand : public QUndoCommand {
public:
    /**
     * @brief A constructor.
     * @param scene Graphics Scene to place a class to.
     * @param params parameters of a class
     * @param parent parent undo command
     */
    explicit AddClassCommand(QGraphicsScene *scene, ClassDiagramItemParameters *params, QUndoCommand *parent = nullptr);

    /**
     * A destructor.
     */
    ~AddClassCommand();

    /**
     * @brief Undo command
     */
    void undo() override;

    /**
     * @brief Redo command
     */
    void redo() override;

private:
    ClassDiagramItem *diagramItem; ///< Class object
    QGraphicsScene *graphicsScene; ///< graphics scene to place a class object to
    QPointF initialStartPosition; ///< initial position of a class object. Chosen randomly.
};

/**
 * Command for adding a connection to the sequence diagram with undo/redo commands suppor.
 */
class AddSequenceConnectionCommand : public QUndoCommand {
public:
    /**
     * @brief A constructor.
     * @param fromNode Starting node
     * @param toNode Ending node
     * @param params Connection parameters
     * @param connectionType Connection type
     * @param scene Graphics Scene
     * @param parent parent undo command
     */
    explicit AddSequenceConnectionCommand(SequenceDiagramItem *fromNode,
                                          SequenceDiagramItem *toNode,
                                          messageParams *params,
                                          ConnectionType connectionType,
                                          QGraphicsScene *scene,
                                          QUndoCommand *parent = nullptr);

    /**
     * A destructor.
     */
    ~AddSequenceConnectionCommand();

    /**
     * @brief Undo command.
     */
    void undo() override;

    /**
     * @brief Redo command
     */
    void redo() override;

private:
    SequenceConnectionItem *actorConnection; ///< a connection object, that will be added to a scene
    QGraphicsScene *graphicsScene; ///< a graphics scene to place a sequence connection to
};

/**
 * Add a connection command to the class diagram command. Fully supports undo/redo commands.
 */
class AddClassConnectionCommand : public QUndoCommand {
public:
    /**
     * @brief A constructor.
     * @param fromNode Node (class) to start from
     * @param toNode Node (class) to end at
     * @param params Connection parameters
     * @param type Connection type
     * @param scene Graphics Scene to place a connection to
     * @param parent Parent undo command
     */
    explicit AddClassConnectionCommand(ClassDiagramItem *fromNode,
                                       ClassDiagramItem *toNode,
                                       ClassDiagramConnectionParams *params,
                                       ClassConnectionItem::ClassConnectionType type,
                                       QGraphicsScene *scene,
                                       QUndoCommand *parent = nullptr);

    /**
     * A destructor.
     */
    ~AddClassConnectionCommand();

    /**
     * @brief Undo command.
     */
    void undo() override;

    /**
     * @brief Redo command.
     */
    void redo() override;

private:
    ClassConnectionItem *classConnection; ///< a connection, that will be added
    QGraphicsScene *graphicsScene; ///< A graphic scene to add a connection to
};

#endif //DIAGRAMS_COMMANDS_H
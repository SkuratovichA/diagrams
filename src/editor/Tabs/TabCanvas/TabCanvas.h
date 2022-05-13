/** @file TabCanvas.h
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 07.05.2022
 */


// File: TabCanvas.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QUndoGroup>
#include <QGraphicsView>

#include "../EditorScene/EditorScene.h"
#include "../Commands/Commands.h"
#include "../DiagramItem/DiagramItem.h"
#include "../FillItems/ObjectParams.h"
#include "ItemsBuffer/Itemsbuffer.h"
#include "../../Backend/Parse.h"

class editorInterface;

/**
 * Macro for adding a signal
 */
#define ADD_SIGNAL(obj, name, icon, shortcut, receiver, memberslot) \
    do {                                                            \
        obj = new QAction(tr((name)), this);                        \
        /*obj->setIcon(icon);*/                                     \
        (obj)->setShortcut(tr(shortcut));                           \
        connect((obj), SIGNAL(triggered()), receiver, memberslot);  \
    } while(0)

/**
 * @brief Tab canvas for editing a diagram (either class or a sequence)
 */
class TabCanvas : public QMainWindow {
Q_OBJECT

// region Constructor, Destructor
public:
    /**
     * @brief A Constructor.
     * @param parent a parent
     * @param parentGroup a parent undo group
     */
    explicit TabCanvas(
            QWidget *parent = nullptr,
            QUndoGroup *parentGroup = nullptr
    ) {
        _undoStack = new QUndoStack(parentGroup);
        buffer = new ItemsBuffer();
    }

    /**
     * @brief A Destructor.
     */
    ~TabCanvas() {
        delete buffer;
        delete editorScene;
    }
// endregion

public /*slots*/:

    /**
     * @brief Pastes all items from "copy" buffer.
     */
    virtual void paste() = 0;

    /**
     * @brief Copy all selected items to the local "copy" buffer from the scene.
     */
    virtual void copy() = 0;

    /**
     * @brief Copy all selected items to the local "copy" buffer from the scene
     * and the delete them.
     */
    virtual void cut() = 0;

    /**
     * @brief Show a context menu with actions for objects.
     * @param pos position on the scene where the click was handled
     */
    virtual void showContextMenu(const QPoint &pos) = 0;

    /**
     * @brief Add a new entity to the sequence diagram with the same name as the parent item.
     * @param classDiagramItemParent pointer to an object from a class diagram scene
     */
    virtual void addEntity() = 0;

    /**
     * @brief Add a connection between actor life lines.
     */
    virtual void addConnection() = 0;

    /**
     * @brief Decrease Z value of the selected items and send them to back.
     */
    virtual void toBack() = 0;

    /**
     * @brief Increase Z value of the selected items and send them to front.
     */
    virtual void toFront() = 0;

// region Implemented methods
    // region methods
private:

    /**
     * @brief Change scale according to scale factor.
     * @param scaleFactor A scale factor.
     */
    void scaleView(qreal scaleFactor) {
        qreal factor = view->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;
        view->scale(scaleFactor, scaleFactor);
    }

    // endregion

    // region Templates
public:

    /**
     * @brief Returns two first selected elements from the scene.
     *
     * @tparam T typename. Must inherit from QGraphicsItem
     * @return Pair of two selected elements with type T.
     */
    template<typename T>
    QPair<T *, T *> getSelectedDiagramItems() {
        auto items = editorScene->selectedItems();
        // remove selected items of another type
        for (auto item: items) {
            if (dynamic_cast<T *>(item) == nullptr) {
                items.removeOne(item);
            }
        }

        if (items.isEmpty()) {
            return QPair<T *, T *>();
        }
        // sort by a position
        std::sort(items.begin(), items.end(),
                  [](QGraphicsItem *a, QGraphicsItem *b) {
                      if (a->x() <= b->y()) {return false;}
                      if (a->y() <= b->y()) {return false;}
                      return true;
                  }
        );

        // take the first element of a list
        auto first = dynamic_cast<T *>(items.first());
        T *rest;
        switch (items.count()) {
            case 2:
                // remove a previously taken element
                items.removeFirst();
                // fallthrough
            case 1:
                rest = dynamic_cast<T *>(items.first());
                if (dynamic_cast<SequenceDiagramItem *>(first) != nullptr && first == rest) {
                    return QPair<T *, T*>();
                }
                break;
            default:
                items.removeFirst();
                rest = dynamic_cast<T *>(items.first());
        }
        return QPair<T *, T *>(first, rest);
    }

    /**CHUJ
     *
     * @tparam T
     * @return
     */
    template<typename T>
    T *selectedObject() {
        auto selectedPair = getSelectedDiagramItems<T>();
        return selectedPair == QPair<T *, T *>() ? nullptr : selectedPair.first;
    }

    /**CHUJ
     *
     * @tparam T
     * @return
     */
    template<typename T>
    QList<T *> getItems() {
        QList<T *> items;
        for (auto x: editorScene->items()) {
            if (dynamic_cast<T *>(x) == nullptr) {
                continue;
            }
            items.push_back(dynamic_cast<T *>(x));
        }
        return items;
    }

    /** CHUJ
     * @brief Sets Z value
     *
     * @tparam T
     * @param items list
     * @param val
     */
    template<typename T>
    void setZvalue(QList<QGraphicsItem *> items, int val) {
        T *ptr1;
        for (auto x: items) {
            ptr1 = dynamic_cast<T *>(x);
            if (ptr1 == nullptr) {
                continue;
            }
            x->setZValue(val);
        }
        editorScene->update();
    }

    /**
     * CHUJ
     */
    template<typename T>
    void _toZchange(bool toFront) {
        QList<QGraphicsItem *> selItems = editorScene->selectedItems();
        QList<QGraphicsItem *> allItems = editorScene->items();
        qreal z = toFront ? 1 : -1;
        setZvalue<T>(selItems, z);
        for (auto x: selItems) {
            allItems.removeOne(x);
        }
        setZvalue<T>(allItems, -z);
    }
    // endregion

    // region Getters, Setters
public:
    /**
     * CHUJ
     */
    QColor color() const {
        return {QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256),
                180};
    }

    /**
     * CHUJ
     */
    QUndoStack *undoStack() const {
        return _undoStack;
    }

    /**
     * CHUJ
     */
    void updateScene() {
        editorScene->update();
    }

    /**
     * CHUJ
     */
    void createScene() {
        editorScene = new EditorScene(this);
        editorScene->setSceneRect(QRect(-50, -50, _sceneWidth, _sceneHeight));
        connect(editorScene, &EditorScene::itemMoved, this, &TabCanvas::moveEntity);

        view = new QGraphicsView(editorScene);
        view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        view->setDragMode(QGraphicsView::RubberBandDrag);
        view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        view->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(view, SIGNAL(customContextMenuRequested(const QPoint&)),
                this, SLOT(showContextMenu(const QPoint&)));
        setCentralWidget(view);
    }
    // endregion

    // region Virtual methods
public:

    /**
     * @brief Generate random [x, y] coordinates ranging from 0 to 1200 with a margin
     *
     * @return QPoint coordinates on the scene for new item
     */
    virtual QPoint generateCoords() const = 0;

    /** CHUJ
     * @brief Gets string representation from given Program
     * @param prg
     */
    virtual bool getStringRepresentation(Program &prg) = 0;
// endregion

public slots:

    /**
     * @brief Moves entity to the given position.
     *
     * @param movedItem item to be moved
     * @param startPosition starting position
     */
    void moveEntity(QGraphicsItem *movedItem, const QPointF &startPosition) {
        _undoStack->push(new MoveCommand(movedItem, startPosition));
    }

    /**
     * @brief Removes entity from scene, pushes into undo stack
     */
    void removeEntity() {
        if (editorScene->selectedItems().isEmpty()) {
            return;
        }
        _undoStack->push(new DeleteCommand(editorScene));
    }

    /**
     * @brief Zooms in with with scale factor of 1.2
     */
    void zoomIn() {
        scaleView(qreal(1.2));
    }

    /**
     * @brief Zooms out with with scale factor of 1.2
     */
    void zoomOut() {
        scaleView(1 / qreal(1.2));
    }

protected:
    EditorScene *editorScene = nullptr; ///< Editor scene
    QUndoStack *_undoStack = nullptr; ///< UNDO stack
    ItemsBuffer *buffer = nullptr; ///< Item buffer

private:
    QGraphicsView *view; ///< Graphics view
    const qreal _sceneWidth = 800; ///< Constant scene width
    const qreal _sceneHeight = 600; ///< Constant scene height
};

/**
 * @brief Canvas for editing class diagram
 */
class ClassCanvas : public TabCanvas {
Q_OBJECT

public:
    /**
     * @brief A constructor.
     * This constructor creates a tab with a class diagram.
     * @param p parent widget
     * @param parentGroup pointer to the main undo stack to create a local undo stack
     */
    ClassCanvas(QWidget *parent = nullptr, QUndoGroup *parentGroup = nullptr);

public:

    /**
     * @brief Implements virtual method getStringRepresentation() from TabCanvas superclass
     */
    bool getStringRepresentation(Program &prg) override;

    /**
     * @brief Implements virtual method createFromFile(dgrmClass_t) from TabCanvas superclass
     */
    bool createFromFile(dgrmClass_t cls);

    /**
     * @brief Get a list with class pointers and its names
     * @return A list of pairs of type <class pointer, its name>
     */
    QList<QPair<ClassDiagramItem *, QString>> getClassStringPairs();

    /**
     * @brief Implements virtual method generateCoords() from TabCanvas superclass
     */
    QPoint generateCoords() const override;

    /**
     * @brief Checks if item names from sequence diagram are corresponding to class ones.
     *
     * @return true in case of correspondence, false otherwise
     */
    bool checkIdenticalNames();

    /**
     * @brief Check permissions for methods and attributes of a class
     * @return true in case of success, otherwise false
     */
    bool checkPermissions();

    /**
     * @brief Compare permissions of two methods
     * @param y A function name (with permissions)
     * @param str String with permissions
     * @return true if permissions are the same
     */
    bool comparePermissions(QGraphicsTextItem *y, QString str);

private:

    /**
     * @brief Create and connect all signals for interaction with classes.
     */
    void createEntityClassContextMenu();

    /**
     * @brief Create and connect all signals for interaction with relation arrows.
     */
    void createConnectionContextMenu();

public slots:

    /**
     * @brief Implements virtual method paste() from TabCanvas superclass
     */
    void paste() override;

    /**
     * @brief Implements virtual method cut() from TabCanvas superclass
     */
    void cut() override;

    /**
     * @brief Implements virtual method copy() from TabCanvas superclass
     */
    void copy() override;

    /**
     * @brief Implements virtual method addEntity() from TabCanvas superclass
     */
    void addEntity() override;

    /**
     * @brief Implements virtual method addConnection() from TabCanvas superclass
     */
    void addConnection() override;

    /**
     * @brief Implements virtual method showContextMenu() from TabCanvas superclass
     */
    void showContextMenu(const QPoint &pos) override;

    /**
     * @brief Implements virtual method toBack() from TabCanvas superclass
     */
    void toBack() override;

    /**
     * @brief Implements virtual method toFront() from TabCanvas superclass
     */
    void toFront() override;

    /**
     * @brief Add a new method to the selected class at the end.
     */
    void addMethod_triggered();

    /**
     * @brief Add a new attribute to the selected class at the end.
     */
    void addAttr_triggered();

    /**
     * @brief Delete the last method from the selected class.
     */
    void rmAttr_triggered();

    /**
     * @brief Delete the last method from the selected class.
     */
    void rmMethod_triggered();

    /**
     * @brief Create an aggregation relation between two selected classes.
     */
    void aggregation_triggered();

    /**
     * @brief Create a composition relation between two selected classes.
     */
    void composition_triggered();

    /**
     * @brief Create a generalization relation between two selected classes.
     */
    void generalization_triggered();

    /**
     * @brief Create an association relation between two selected classes.
     */
    void association_triggered();

    /**
     * @brief Create a dependency relation between two selected classes.
     */
    void dependency_triggered();

    /**
     * @brief Change the orientation of the arrow.
     */
    void orientation_triggered();

private:
    ClassDiagramItemParameters *createItem; ///< Parameters for creating a class diagram object
    relationsParams *createRelation; ///< Parameters for creating a relation
    QMenu *classMenu; ///< Class menu
    QMenu *connectionMenu; ///< Connection menu

    QAction *addMethod; ///< Action for method addition
    QAction *rmMethod; ///< Action for method removal
    QAction *addAttr; ///< Action for attribute addition
    QAction *rmAttr; ///< Action for attribute removal

    QAction *aggregation; ///< Aggregation relation
    QAction *composition; ///< Composition relation
    QAction *generalization; ///< Generalization relation
    QAction *association; ///< Association relation
    QAction *dependency; ///< Dependency relation
    QAction *orientation; ///< Orientation of a relation (connection)
};

/**
 * @brief A canvas for editing sequence diagram
 */
class SequenceCanvas : public TabCanvas {
Q_OBJECT

public:
    /**
     * A constructor.
     *
     * This constructor creates a tab with a sequence diagram.
     *
     * @param p parent widget
     * @param parentGroup pointer to the main undo stack
     * to create a local undo stack
     */
    SequenceCanvas(QWidget *parent = nullptr, QUndoGroup *parentGroup = nullptr);

public slots:

    /**
     * @brief Implements virtual method paste() from TabCanvas superclass
     */
    void paste() override;

    /**
     * @brief Implements virtual method cut() from TabCanvas superclass
     */
    void cut() override;

    /**
     * @brief Implements virtual method copy() from TabCanvas superclass
     */
    void copy() override;

    /**
     * @brief Implements virtual method addEntity() from TabCanvas superclass
     */
    void addEntity() override {};

    /**
     * @brief Implements virtual method addEntity() from TabCanvas superclass
     */
    void addEntity(ClassDiagramItem *classDiagramItemParent);

    /**
     * @brief Implements virtual method addConnection() from TabCanvas superclass
     */
    void addConnection() override;

    /**
     * @brief Implements virtual method showContextMenu() from TabCanvas superclass
     */
    void showContextMenu(const QPoint &pos) override;

    /**
     * @brief Implements virtual method toBack() from TabCanvas superclass
     */
    void toBack() override;

    /**
     * @brief Implements virtual method toFront() from TabCanvas superclass
     */
    void toFront() override;

    /**
     * @brief Create an asynchronous message between two objects.
     */
    void asynchronousMessage_triggered();

    /**
     * @brief Create a synchronous message between two objects.
     */
    void synchronousMessage_triggered();

    /**
     * @brief Create a return message between two objects.
     */
    void returnMessage_triggered();

    /**
     * @brief Create a message for creating a new object.
     */
    void createMessage_triggered();

    /**
     * @brief Create a message for deleting an object.
     */
    void deleteMessage_triggered();

public:

    /**
     * @brief Implements virtual method getStringRepresentation() from TabCanvas superclass
     */
    bool getStringRepresentation(Program &prg) override;

    /**
     * @brief Creates a diagram from the given file
     * @param seq
     * @return true in case of success, false otherwise
     */
    bool createFromFile(dgrmSeq_t seq);

    /**
     * @brief Implements virtual method generateCoords() from TabCanvas superclass
     */
    QPoint generateCoords() const override;

    /**
     * @brief Checks if item names from sequence diagram are corresponding to class ones.
     * @return true in case of correspondence, false otherwise
     */
    bool checkIdenticalNames();

private:

    /**
     * @brief Creates sequence's context menu
     */
    void createSequenceContextMenu();

private:
    QList<QPair<ClassDiagramItem *, SequenceDiagramItem *>> pairClassSequence =
            QList<QPair<ClassDiagramItem *, SequenceDiagramItem *>>(); ///< List of pairs of classes and its children from sequence diagram
    QMenu *sequenceMenu = nullptr; ///< Sequence connection mesu
    QAction *asynchronousMessage = nullptr; ///< Asynchronous message
    QAction *synchronousMessage = nullptr; ///< Synchronous message
    QAction *returnMessage = nullptr; ///< Return message
    QAction *createMessage = nullptr; ///< Create message
    QAction *deleteMessage = nullptr; ///< Delete message
    SequenceDiagramItemParameters *createActor = nullptr; ///< Parameters to create a sequence connection
    messageParams *paramsMessage = nullptr; ///< Parameters of a sequence connection (message)
    editorInterface *parentInterface = nullptr; ///< Parent editor interface
};

#endif // TABCANVAS_H

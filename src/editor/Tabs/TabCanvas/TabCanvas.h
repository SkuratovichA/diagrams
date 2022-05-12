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
#include "../FillItems/ObjectParams.h" // can delete this maybe
#include "ItemsBuffer/Itemsbuffer.h"
#include "../../Backend/Parse.h"

class editorInterface;

#define ADD_SIGNAL(obj, name, icon, shortcut, receiver, memberslot) \
    do {                                                            \
        obj = new QAction(tr((name)), this);                        \
        /*obj->setIcon(icon);*/                                     \
        (obj)->setShortcut(tr(shortcut));                           \
        connect((obj), SIGNAL(triggered()), receiver, memberslot);  \
    } while(0)

/**
 * @brief
 */
class TabCanvas : public QMainWindow {
Q_OBJECT

// region Constructor, Destructor
public:
    explicit TabCanvas(
            QWidget *parent = nullptr,
            QUndoGroup *parentGroup = nullptr
    ) {

        _undoStack = new QUndoStack(parentGroup);
        buffer = new ItemsBuffer();
    }

    ~TabCanvas() {
        delete buffer;
        delete editorScene;
    }
// endregion

public /*slots*/:

    /**
     * Pastes all items from "copy" buffer.
     */
    virtual void paste() = 0;
    /**
     * Copy all selected items to the local "copy" buffer from the scene.
     */
    virtual void copy() = 0;
    /**
     * Copy all selected items to the local "copy" buffer from the scene
     * and the delete them.
     */
    virtual void cut() = 0;
    /**
     * Show a context menu with actions for objects.
     *
     * @param pos position on the scene where the click was handled
     */
    virtual void showContextMenu(const QPoint &pos) = 0;
    /**
     * Add a new entity to the sequence diagram with the same name as the parent item.
     *
     * @param classDiagramItemParent pointer to an object from a class diagram scene
     */
    virtual void addEntity() = 0;
    /**
     * Add a connection between actor life lines.
     */
    virtual void addConnection() = 0;
    /**
     * Decrease Z value of the selected items and send them to back.
     */
    virtual void toBack() = 0;
    /**
     * Increase Z value of the selected items and send them to front.
     */
    virtual void toFront() = 0;

// region Implemented methods
    // region methods
private:

    /**
     * @brief Change scale according to scale factor
     *
     * @param scaleFactor
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
     * @tparam T typename. Must be inherit from QGraphicsItem
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
                // take the "rest of a list"
                rest = dynamic_cast<T *>(items.first());
                break;
            default:
                items.removeFirst();
                rest = dynamic_cast<T *>(items.first());
        }
        return QPair<T *, T *>(first, rest);
    }

    template<typename T>
    T *selectedObject() {
        auto selectedPair = getSelectedDiagramItems<T>();
        return selectedPair == QPair<T *, T *>() ? nullptr : selectedPair.first;
    }

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

    // region Geters, Setters
public:
    QColor color() const {
        return {QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256),
                180};
    }

    QUndoStack *undoStack() const {
        return _undoStack;
    }

    void updateScene() {
        editorScene->update();
    }

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
// endregion

public slots:

    void moveEntity(QGraphicsItem *movedItem, const QPointF &startPosition) {
        _undoStack->push(new MoveCommand(movedItem, startPosition));
    }

    void removeEntity() {
        if (editorScene->selectedItems().isEmpty()) {
            return;
        }
        _undoStack->push(new DeleteCommand(editorScene));
    }

    void zoomIn() {
        scaleView(qreal(1.2));
    }

    void zoomOut() {
        scaleView(1 / qreal(1.2));
    }

protected:
    EditorScene *editorScene = nullptr;
    QUndoStack *_undoStack = nullptr;
    ItemsBuffer *buffer = nullptr;

private:
    QGraphicsView *view;
    const qreal _sceneWidth = 800;
    const qreal _sceneHeight = 600;
};

class ClassCanvas : public TabCanvas {
Q_OBJECT

public:
    ClassCanvas(QWidget *parent = nullptr, QUndoGroup *parentGroup = nullptr);

public:

    /**
     * Implements virtual method getStringRepresentation() from TabCanvas superclass
     */
    bool getStringRepresentation(Program &prg) override;
    bool createFromFile(dgrmClass_t cls);
    QList<QPair<ClassDiagramItem *, QString>> getClassStringPairs();
    /**
     * Implements virtual method generateCoords() from TabCanvas superclass
     */
    QPoint generateCoords() const override;
    bool checkIdenticalNames();
    bool checkPermissions();
    bool comparePermissions(QGraphicsTextItem *y, QString str);

private:
    void createEntityClassContextMenu();
    void createConnectionContextMenu();

public slots:
    /**
     * Implements virtual method paste() from TabCanvas superclass
     */
    void paste() override;
    /**
     * Implements virtual method cut() from TabCanvas superclass
     */
    void cut() override;
    /**
     * Implements virtual method copy() from TabCanvas superclass
     */
    void copy() override;
    /**
     * Implements virtual method addEntity() from TabCanvas superclass
     */
    void addEntity() override;
    /**
     * Implements virtual method addConnection() from TabCanvas superclass
     */
    void addConnection() override;
    /**
     * Implements virtual method showContextMenu() from TabCanvas superclass
     */
    void showContextMenu(const QPoint &pos) override;
    /**
     * Implements virtual method toBack() from TabCanvas superclass
     */
    void toBack() override;
    /**
     * Implements virtual method toFront() from TabCanvas superclass
     */
    void toFront() override;

    void addMethod_triggered();
    void addAttr_triggered();
    void rmAttr_triggered();
    void rmMethod_triggered();

    void aggregation_triggered();
    void composition_triggered();
    void generalization_triggered();
    void association_triggered();
    void dependency_triggered();
    void orientation_triggered();

private:
    classParams *createItem;
    relationsParams *createRelation;
    QMenu *classMenu;
    QMenu *connectionMenu;

    QAction *addMethod;
    QAction *rmMethod;
    QAction *addAttr;
    QAction *rmAttr;

    QAction *aggregation;
    QAction *composition;
    QAction *generalization;
    QAction *association;
    QAction *dependency;
    QAction *orientation;
};

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
     * Implements virtual method paste() from TabCanvas superclass
     */
    void paste() override;
    /**
     * Implements virtual method cut() from TabCanvas superclass
     */
    void cut() override;
    /**
     * Implements virtual method copy() from TabCanvas superclass
     */
    void copy() override;
    /**
     * Implements virtual method addEntity() from TabCanvas superclass
     */
    void addEntity() override {};
    /**
     * Implements virtual method addEntity() from TabCanvas superclass
     */
    void addEntity(ClassDiagramItem *classDiagramItemParent);
    /**
     * Implements virtual method addConnection() from TabCanvas superclass
     */
    void addConnection() override;
    /**
     * Implements virtual method showContextMenu() from TabCanvas superclass
     */
    void showContextMenu(const QPoint &pos) override;
    /**
     * Implements virtual method toBack() from TabCanvas superclass
     */
    void toBack() override;
    /**
     * Implements virtual method toFront() from TabCanvas superclass
     */
    void toFront() override;
    /**
     * Create an asynchronous message between two objects.
     */
    void asynchronousMessage_triggered();
    /**
     * Create a synchronous message between two objects.
     */
    void synchronousMessage_triggered();
    /**
     * Create a return message between two objects.
     */
    void returnMessage_triggered();
    /**
     * Create a message for creating a new object.
     */
    void createMessage_triggered();
    /**
     * Create a message for deleting an object.
     */
    void deleteMessage_triggered();

public:

    /**
     * Implements virtual method getStringRepresentation() from TabCanvas superclass
     */
    bool getStringRepresentation(Program &prg) override;

    /** CHUJ
     * @brief Creates sequence tab from given file
     * @param seq
     * @return bool error?
     */
    bool createFromFile(dgrmSeq_t seq);
    /**
     * Implements virtual method generateCoords() from TabCanvas superclass
     */
    QPoint generateCoords() const override;
    bool checkIdenticalNames();

private:
    void createSequenceContextMenu();

private:

    QList<QPair<ClassDiagramItem *, SequenceDiagramItem *>> pairClassSequence =
            QList<QPair<ClassDiagramItem *, SequenceDiagramItem *>>();
    QMenu *sequenceMenu = nullptr;
    QAction *asynchronousMessage = nullptr;
    QAction *synchronousMessage = nullptr;
    QAction *returnMessage = nullptr;
    QAction *createMessage = nullptr;
    QAction *deleteMessage = nullptr;
    SequenceDiagramItemParameters *createActor = nullptr;
    messageParams *paramsMessage = nullptr;
    editorInterface *parentInterface = nullptr;

};

#endif // TABCANVAS_H

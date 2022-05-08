#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QUndoGroup>
#include <QGraphicsView>

#include "../DiagramItem/EditorScene/EditorScene.h"
#include "../DiagramItem/Commands/Commands.h"
#include "../DiagramItem/DiagramItem.h"
#include "../DiagramItem/FillItems/ObjectParams.h"
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
    virtual void paste() = 0;
    virtual void copy() = 0;
    virtual void cut() = 0;
    virtual void showContextMenu(const QPoint &pos) = 0;
    virtual void addEntity() = 0;
    virtual void addConnection() = 0;
    virtual void toBack() = 0;
    virtual void toFront() = 0;

// region Implemented methods
    // region methods
private:

    void scaleView(qreal scaleFactor) {
        qreal factor = view->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;
        view->scale(scaleFactor, scaleFactor);
    }
    // endregion

    // region Templates
public:
    template<typename T>
    QPair<T *, T *> getSelectedDiagramItems() {
        auto items = editorScene->selectedItems();
        if (items.isEmpty()) {
            return QPair<T *, T *>();
        }
        QList<T *> listl;
        auto first = dynamic_cast<T *>(items.first());
        T *rest = nullptr;

        switch (items.count()) {
            case 2:
                items.removeFirst();
                // fallthrough
            case 1:
                rest = dynamic_cast<T *>(items.first());
                if (first == nullptr || rest == nullptr) {
                    qDebug() << "shit fuck";
                    return QPair<T *, T *>();
                }
                break;
            default:
                qDebug() << items.count() << " nodes were selected. Bad";
                        foreach(auto item, items) {
                        if (dynamic_cast<T *>(item) != nullptr) {
                            listl.append(dynamic_cast<T * >(item));
                        }
                    }
                qDebug() << listl.count() << " elements to select.";
                first = listl.first();
                listl.removeFirst();
                rest = listl.first();
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

    void createScene() {
        editorScene = new EditorScene(this);
        editorScene->setSceneRect(QRect(0, 0, _sceneWidth, _sceneHeight));
        connect(editorScene, &EditorScene::itemMoved, this, &TabCanvas::moveEntity);
        view = new QGraphicsView(editorScene);
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
    virtual QPoint generateCoords() const = 0;
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
        QUndoCommand *deleteCommand = new DeleteCommand(editorScene);
        _undoStack->push(deleteCommand);
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
//    EditorInterface *editorInterface = nullptr;
    ItemsBuffer *buffer = nullptr;

private:
    QGraphicsView *view;

    qreal _sceneWidth = 600;
    qreal _sceneHeight = 600;

};

class ClassCanvas : public TabCanvas {
Q_OBJECT

public:
    ClassCanvas(QWidget *parent = nullptr, QUndoGroup *parentGroup = nullptr);

public:
    bool getStringRepresentation(Program &prg) override;
    QList<QPair<ClassDiagramItem *, QString>> getClassStringPairs();
    QPoint generateCoords() const override;

private:
    void createEntityClassContextMenu();
    void createConnectionContextMenu();

public slots:
    void paste() override;
    void cut() override;
    void copy() override;
    void addEntity() override;
    void addConnection() override;
    void showContextMenu(const QPoint &pos) override;
    void toBack() override;
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
    SequenceCanvas(QWidget *parent = nullptr, QUndoGroup *parentGroup = nullptr);

public slots:

    void paste() override;

    void cut() override;

    void copy() override;

    void addEntity() override {};
    void addEntity(ClassDiagramItem *classDiagramItemParent);

    void addConnection() override;

    void showContextMenu(const QPoint &pos) override;

    void toBack() override;

    void toFront() override;

    void asynchronousMessage_triggered();
    void synchronousMessage_triggered();
    void returnMessage_triggered();
    void createMessage_triggered();
    void deleteMessage_triggered();

public:
    bool getStringRepresentation(Program &prg) override;
    QPoint generateCoords() const override;

private:
    void createSequenceContextMenu();

private:
    QList<QPair<ClassDiagramItem *, SequenceDiagramItem *>> pairClassSequence = QList<QPair<ClassDiagramItem *, SequenceDiagramItem *>>();
    QMenu *sequenceMenu = nullptr;

    QAction *asynchronousMessage = nullptr;
    QAction *synchronousMessage = nullptr;
    QAction *returnMessage = nullptr;
    QAction *createMessage = nullptr;
    QAction *deleteMessage = nullptr;
    actorParams *createActor = nullptr;

    editorInterface *parentInterface = nullptr;
};

#endif // TABCANVAS_H

#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QUndoGroup>
#include <QGraphicsView>

#include "../diagram/EditorScene.h"
#include "../diagram/Commands.h"
#include "../diagram/DiagramItem.h"
#include "../fillItems.h"
#include "../Itemsbuffer.h"

class TabCanvas : public QMainWindow {
Q_OBJECT

// region Constructor, Destructor
public:
    explicit TabCanvas(
            QWidget *parent = nullptr,
            QUndoGroup *parentGroup = nullptr
    ) {
        _undoStack = new QUndoStack(parentGroup);
        createScene();
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
    void createScene() {
        editorScene = new EditorScene(this);
        editorScene->setSceneRect(QRect(0, 0, _sceneWidth, _sceneHeight));
        connect(editorScene, SIGNAL(EditorScene::itemMoved), this, SLOT(TabCanvas::moveEntity));
        view = new QGraphicsView(editorScene);
        view->setDragMode(QGraphicsView::RubberBandDrag);
        view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        view->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(view, SIGNAL(customContextMenuRequested(const QPoint&)),
                this, SLOT(ShowContextMenu(const QPoint&)));
        setCentralWidget(view);
    }

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
    // endregion

    // region Virtual methods
public:
    virtual QPoint generateCoords() const = 0;
    virtual std::string getStringRepresentation() const = 0;
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
    ItemsBuffer *buffer;

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
    std::string getStringRepresentation() const override {return {"hui"};}

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
    SequenceCanvas(QWidget *parent = nullptr, QUndoGroup *parentGroup = nullptr) {}

public slots:

    void paste() override {};

    void cut() override {};

    void copy() override {};

    void addEntity() override {};

    void addConnection() override {};

    void showContextMenu(const QPoint &pos) override {};

    virtual void toBack() override {};

    virtual void toFront() override {};

public:
    std::string getStringRepresentation() const override {return {"hui"};};

    QPoint generateCoords() const override {
        return QPoint(QRandomGenerator::global()->bounded(600), 30);
    }
};

#endif // TABCANVAS_H

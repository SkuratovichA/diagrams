#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QUndoGroup>
#include <QGraphicsView>
#include "diagram/EditorScene.h"
#include "diagram/DiagramItem.h"
#include "fillItems.h"
#include "Itemsbuffer.h"

namespace SceneType {
    enum DiagramType {
        SEQUENCE,
        CLASS,
        DIAGRAM_TYPE_DEFAULT,
    };
}

using namespace SceneType;

class TabCanvas : public QMainWindow {
Q_OBJECT

    /** Public constructor, destructor
     *
     */
public:
    explicit TabCanvas(
            QWidget *parent = nullptr,
            DiagramType diagramType = DIAGRAM_TYPE_DEFAULT,
            QUndoGroup *parentGroup = nullptr
    );

    ~TabCanvas();

    /** Public functions
     *
     */
public:
    QUndoStack *getUndoStack() const {
        return undoStack;
    }

    std::string getStringRepresentation();

    QGraphicsView *view;

    void scaleView(qreal scaleFactor);

private:
    template<typename T>
    QPair<T *, T *> getSelectedDiagramItems();

    QGraphicsItem *selectedObject();

    void createMenusClass();
    void createMenusConnections();

    QColor generateColor() {
        return QColor(QRandomGenerator::global()->bounded(256),
                      QRandomGenerator::global()->bounded(256),
                      QRandomGenerator::global()->bounded(256),
                      180);
    }

    QPoint generateCoords() {
        return QPoint(QRandomGenerator::global()->bounded(600),
                      30);
    }

    void setZvalue(int forSelect, int forOther);

private:
    DiagramType type;
    QUndoStack *undoStack = nullptr;
    EditorScene *editorScene = nullptr;

    QMenu *classMenu;
    QAction *addMethod;
    QAction *rmMethod;
    QAction *addAttr;
    QAction *rmAttr;

    QMenu *connectionMenu;
    QAction *aggregation;
    QAction *composition;
    QAction *generalization;
    QAction *association;
    QAction *dependency;
    QAction *orientation;

    ItemsBuffer *buffer;
    classParams *createItem;
    actorParams *createActor;

    /** Slots
     */
public slots:
    void ShowContextMenu(const QPoint& pos); // this is a slot
    void moveEntity(ActorDiagramItem *movedItem, const QPointF &startPosition);
    void removeEntity();
    void addEntity();
    void addConnection();

    void cut();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void toBack();
    void toFront();

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

    void createScene();
};

#endif // TABCANVAS_H

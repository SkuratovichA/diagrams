#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include <QMainWindow>
#include "diagram/EditorScene.h"


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

public:
    explicit TabCanvas(
            QWidget *parent = nullptr,
            DiagramType diagramType = DIAGRAM_TYPE_DEFAULT,
            QUndoGroup *parentGroup = nullptr
    );

    ~TabCanvas();

public:
    QUndoStack *getUndoStack();

    std::string getStringRepresentation();


    /** Private functions
     */
private:

    QGraphicsItem *selectedObject();

    /** Private variables
     */
private:
    DiagramItem::EntityType newEntityType = DiagramItem::UNKNOWN_TYPE;
    DiagramItem::EntityType newConnectionType = DiagramItem::UNKNOWN_TYPE;

    QUndoStack *undoStack = nullptr;
    //Diagram *diagram = nullptr;
    QVBoxLayout *layout = nullptr;

//    QGraphicsView *view = nullptr;
//    QGraphicsScene *scene = nullptr;

    EditorScene *editorScene = nullptr;

    /** Slots
     */
public slots:
    void moveEntity(DiagramItem *movedItem, const QPointF &startPosition);
    void removeEntity();
    void addEntity();
    void addConnection();

    void cut();
    void copy();
    void paste();
    void properties();
    void sendToBack();
    void sendToFront();


    void createScene();
};

#endif // TABCANVAS_H

#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QUndoGroup>
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
    DiagramType type;
    QUndoStack *undoStack = nullptr;
    QVBoxLayout *layout = nullptr;
    EditorScene *editorScene = nullptr;

    QMenu *editMenu;
    //QToolBar *editToolBar;
    QAction *addMethod;
    QAction *rmMethod;
    QAction *addAttr;
    QAction *rmAttr;

    /** Slots
     */
public slots:
    void moveEntity(ActorDiagramItem *movedItem, const QPointF &startPosition);
    void removeEntity();
    void addEntity();
    void addConnection();

    void cut();
    void copy();
    void paste();
    void properties();
    void sendToBack();
    void sendToFront();

    void addMethod_triggered();
    void addAttr_triggered();
    void rmAttr_triggered();
    void rmMethod_triggered();

    void createScene();
};

#endif // TABCANVAS_H

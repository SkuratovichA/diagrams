#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include "diagram/Diagrams.h"


namespace SceneType {
    enum DiagramType {
        SEQUENCE,
        CLASS,
        DIAGRAM_TYPE_DEFAULT,
    };
}

using namespace SceneType;

class TabCanvas : public QWidget {
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
    void create_scene();

    QGraphicsItem *selectedObject();

    /** Private variables
     */
private:
    QUndoStack *undoStack = nullptr;
    Diagram *diagram = nullptr;
    QVBoxLayout *layout = nullptr;
    QGraphicsView *view = nullptr;
    QGraphicsScene *scene = nullptr;

/** Slots
 */
public slots:

    void addEntity();

    void addConnection();

    void remove();

    void undo();

    void redo();

    void cut();

    void copy();

    void paste();

    void properties();

    void sendToBack();

    void sendToFront();


};

#endif // TABCANVAS_H

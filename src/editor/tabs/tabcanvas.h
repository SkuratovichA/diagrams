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
    QUndoStack *getUndoStack();

    std::string getStringRepresentation();


    /** Private functions
     */
private:
    template<typename T>
    QPair<T *, T *> getSelectedDiagramItems();


    QGraphicsItem *selectedObject();

    /** Private variables
     */
private:
    DiagramType type;

    QUndoStack *undoStack = nullptr;
    QVBoxLayout *layout = nullptr;
    EditorScene *editorScene = nullptr;

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

    void createScene();
};

#endif // TABCANVAS_H

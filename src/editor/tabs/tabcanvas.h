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

class TabCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit TabCanvas(QWidget *parent = nullptr, DiagramType diagramType = DIAGRAM_TYPE_DEFAULT);
    ~TabCanvas();

public:
    std::string get_string_representation();

private:
    void create_scene();

private:
    Diagram* diagram;
    QVBoxLayout *layout;
    QGraphicsView *view;
    QGraphicsScene *scene;

/** Slots
 */
public slots:
    void addEntity();
    void addConnection();
    void remove();
    void cut();
    void copy();
    void paste();
    void properties();
    void sendToBack();
    void sendToFront();


};

#endif // TABCANVAS_H

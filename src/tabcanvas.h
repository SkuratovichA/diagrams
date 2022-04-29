#ifndef TABCANVAS_H
#define TABCANVAS_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>


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

private:

};

#endif // TABCANVAS_H

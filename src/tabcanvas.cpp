#include "tabcanvas.h"

using namespace SceneType;

TabCanvas::TabCanvas(QWidget *parent, DiagramType type) :
        QWidget(parent) {
    // TODO: create a scene here
}

TabCanvas::~TabCanvas() {
}

/**
 *
 */
void TabCanvas::addEntity() {
    qDebug() << "not segfault";
}

/**
 *
 */
void TabCanvas::addConnection() {

}

/**
 *
 */
void TabCanvas::remove() {

}

/**
 *
 */
void TabCanvas::cut() {

}

/**
 *
 */
void TabCanvas::copy() {

}

/**
 *
 */
void TabCanvas::paste() {

}

/**
 *
 */
void TabCanvas::properties() {

}

/**
 *
 */
void TabCanvas::sendToBack() {

}

void TabCanvas::sendToFront() {

}

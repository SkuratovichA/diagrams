#ifndef Object_H
#define Object_H

#include <QString>
#include <QColor>
#include <QGraphicsItem>
#include <QApplication>
#include <QSet>
#include <QKeyEvent>
#include <QRandomGenerator>

#include <QGraphicsPolygonItem>
#include "Connections.h"

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPointF;
QT_END_NAMESPACE


class DiagramItem {
public:
    enum DiagramType {
        Actor, Class
    };

    explicit DiagramItem(qreal width, qreal height, DiagramType type) {
        _height = height;
        _width = width;
        _type = type;
        _boundingBox = QRectF(0, 0, _width, _height);
        _color = QColor(QRandomGenerator::global()->bounded(256),
                        QRandomGenerator::global()->bounded(256),
                        QRandomGenerator::global()->bounded(256), 180);
    }

    DiagramType type() {
        return _type;
    }

    QRectF boundingBox() {
        return _boundingBox;
    }

    void setBoundingBox(QRectF box) {
        _boundingBox = box;
    }

    void setBoundingBox(qreal a, qreal b, qreal c, qreal d) {
        _boundingBox = QRectF(a, b, c, d);
    }

    QString typeStr() {
        return type() == Actor ? QString("Actor") : QString("Class");
    }

    qreal height() {
        return _height;
    }

    void setWidth(qreal width) {
        _width = width;
    }

    void setHeight(qreal height) {
        _height = height;
    }

    void setColor(QColor color) {
        _color = color;
    }

    void setColor(qreal r, qreal g, qreal b, qreal a) {
        _color = QColor(r, g, b, a);
    }

    void setColor(qreal r, qreal g, qreal b) {
        _color = QColor(r, g, b);
    }

    QColor color() {
        return _color;
    }

    qreal width() {
        return _width;
    }

private:
    qreal _height;
    qreal _width;
    QRectF _boundingBox;
    DiagramType _type;
    QColor _color;
};

class ActorDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ActorDiagramItem(QGraphicsItem *item = nullptr);

    void addConnection(ActorConnectionItem *connection);

    void removeConnection(ActorConnectionItem *connection);

private:
    QSet<ActorConnectionItem *> connections;
};

class ClassDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ClassDiagramItem(QGraphicsItem *item = nullptr);

    void refactorTable();

    void addConnection(ClassConnectionItem *connection);

    void removeConnection(ClassConnectionItem *connection);

    QList<QGraphicsTextItem *> getMethods() {
        return methods;
    }

    QList<QGraphicsLineItem *> getMethodsLines() {
        return methodsLines;
    }

    void popMethod() {
        if (methods.isEmpty()) {
            return;
        }
        QGraphicsTextItem *tmp = methods.takeLast();
        delete tmp;
    }

    void popMethodsLine() {
        if (methodsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = methodsLines.takeLast();
        delete tmp;
    }

    void pushAttr(QGraphicsTextItem *item) {
        attrs.push_back(item);
    }

    void pushMethod(QGraphicsTextItem *item) {
        methods.push_back(item);
    }

    void pushMethodLine(QGraphicsLineItem *item) {
        methodsLines.push_back(item);
    }

    void pushAttrLine(QGraphicsLineItem *item) {
        attrsLines.push_back(item);
    }

    void setTextFlags(QGraphicsTextItem *item) {
        item->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditable     |
                                      Qt::TextInteractionFlag::TextSelectableByMouse |
                                      Qt::TextInteractionFlag::TextSelectableByKeyboard);
    }

private:
    QSet<ClassConnectionItem *> connections;
    QList<QGraphicsTextItem *> attrs;
    QList<QGraphicsTextItem *> methods;
    QList<QGraphicsLineItem *> attrsLines;
    QList<QGraphicsLineItem *> methodsLines;
    QColor customColor;
};

#endif // Object_H

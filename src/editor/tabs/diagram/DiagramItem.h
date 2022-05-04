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
#include "../fillItems.h"

QT_BEGIN_NAMESPACE
class QGraphicsItem;

class QGraphicsScene;

class QGraphicsSceneMouseEvent;

class QPointF;

QT_END_NAMESPACE

// for attrs and table expanding
class CustomAttrText : public QGraphicsTextItem {
public:
    CustomAttrText(ClassDiagramItem *p, QString text, qreal x, qreal y, QFlags<Qt::TextInteractionFlag> flags);
    ~CustomAttrText();

    ClassDiagramItem *parent() {
        return _parent;
    }

protected:
    void keyReleaseEvent(QKeyEvent *event);

private:
    ClassDiagramItem *_parent;
};

class NameObject : public QGraphicsTextItem {
public:
    NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y, QString str);
    ~NameObject();

    QGraphicsItem *parent() {
        return _parent;
    }

protected:
    void keyReleaseEvent(QKeyEvent *event);

    QGraphicsItem *_parent;
};

class DiagramItem {
public:
    enum DiagramType {
        Actor, Class
    };

    explicit DiagramItem(qreal width, qreal height, DiagramType type, QColor color) {
        _height = height;
        _width = width;
        _type = type;
        _boundingBox = QRectF(0.0, 0.0, _width, _height);
        _color = color;
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

    qreal width() {
        return _width;
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

    NameObject *_head;
private:
    qreal _height;
    qreal _width;
    QRectF _boundingBox;
    DiagramType _type;
    QColor _color;
};

class ActorDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ActorDiagramItem(actorParams *params);

    void addConnection(ActorConnectionItem *connection);

    void removeConnection(ActorConnectionItem *connection);

private:
    QSet<ActorConnectionItem *> _connections;
};

class ClassDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ClassDiagramItem(classParams *params);
    ~ClassDiagramItem();

    void addConnection(ClassConnectionItem *connection);

    void removeConnection(ClassConnectionItem *connection);

    QPointF centre() {
        return {x() + width() / 2.0, y() + height() / 2.0};
    }

    QList<CustomAttrText *> methods() {
        return _methods;
    }

    QList<QGraphicsLineItem *> methodsLines() {
        return _methodsLines;
    }

    QList<CustomAttrText *> attrs() {
        return _attrs;
    }

    QList<QGraphicsLineItem *> attrsLines() {
        return _attrsLines;
    }

    void popMethod() {
        if (_methods.isEmpty()) {
            return;
        }
        CustomAttrText *tmp = _methods.takeLast();
        delete tmp;
    }

    void popMethodsLine() {
        if (_methodsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = _methodsLines.takeLast();
        delete tmp;
    }

    void popAttr() {
        if (_attrs.isEmpty()) {
            return;
        }
        CustomAttrText *tmp = _attrs.takeLast();
        delete tmp;
    }

    void popAttrsLine() {
        if (_attrsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = _attrsLines.takeLast();
        delete tmp;
    }

    void pushAttr(CustomAttrText *item) {
        _attrs.push_back(item);
    }

    void pushMethod(CustomAttrText *item) {
        _methods.push_back(item);
    }

    void pushMethodLine(QGraphicsLineItem *item) {
        _methodsLines.push_back(item);
    }

    void pushAttrLine(QGraphicsLineItem *item) {
        _attrsLines.push_back(item);
    }

    qreal rowHeight() const {
        return _rowHeight;
    }

    qreal rowWidth() const {
        return _rowWidth;
    }

    qreal tabText() const {
        return _tabText;
    }

    void moveLines(int st, long long el) {
                foreach (QGraphicsLineItem *val, _methodsLines) {
                val->setPos(0, (el + st) * _rowHeight);
                st++;
            }
    }

    void moveTexts(int st, long long el) {
                foreach (CustomAttrText *val, _methods) {
                val->setPos(0, (el + st) * _rowHeight + _tabText);
                st++;
            }
    }

    QGraphicsLineItem *createLine(qreal x, qreal y) {
        auto line = new QGraphicsLineItem(0, 0, _rowWidth, 0, this);
        line->setPos(x, y);
        return line;
    }

    QGraphicsTextItem *createText(qreal x, qreal y, QString text) {
        auto attr = new QGraphicsTextItem(text, this);
        setTextFlags(attr);
        attr->setPos(x, y);
        return attr;
    }

    QFlags<Qt::TextInteractionFlag> flags() {
        return _flags;
    }

    void setTextFlags(QGraphicsTextItem *item) {
        item->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditable |
                                      Qt::TextInteractionFlag::TextSelectableByMouse |
                                      Qt::TextInteractionFlag::TextSelectableByKeyboard);
    }

    QSet<ClassConnectionItem *> connections() const {
        return _connections;
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QSet<ClassConnectionItem *> _connections;
    QList<CustomAttrText *> _attrs;
    QList<CustomAttrText *> _methods;
    QList<QGraphicsLineItem *> _attrsLines;
    QList<QGraphicsLineItem *> _methodsLines;
    qreal _rowHeight;
    qreal _rowWidth;
    qreal _tabText;
    QFlags<Qt::TextInteractionFlag> _flags;
};

#endif // Object_H

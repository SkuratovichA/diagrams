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

// for attrs and table expanding
class CustomAttrText : public QGraphicsTextItem {
public:
    CustomAttrText(ClassDiagramItem *p, QString text, qreal x, qreal y, QFlags<Qt::TextInteractionFlag> flags);
    ~CustomAttrText();

    ClassDiagramItem* parent() {
        return _parent;
    }

protected:
    void keyReleaseEvent(QKeyEvent *event);

private:
    ClassDiagramItem *_parent;
};

class NameObject : public QGraphicsTextItem {
public:
    NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y);
    ~NameObject();

    QGraphicsItem* parent() {
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

    explicit DiagramItem(qreal width, qreal height, DiagramType type) {
        _height = height;
        _width = width;
        _type = type;
        _boundingBox = QRectF(0.0, 0.0, _width, _height);
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
    explicit ActorDiagramItem(QGraphicsItem *item = nullptr);

    void addConnection(ActorConnectionItem *connection);

    void removeConnection(ActorConnectionItem *connection);

private:
    QSet<ActorConnectionItem *> connections;
};

class ClassDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ClassDiagramItem(QGraphicsItem *item = nullptr);
    ~ClassDiagramItem();

    void refactorTable();

    void addConnection(ClassConnectionItem *connection);

    void removeConnection(ClassConnectionItem *connection);

    QPointF centre() {
        return {x() + width()/2.0, y() + height()/2.0};
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QList<CustomAttrText *>  getMethods() {
        return methods;
    }

    QList<QGraphicsLineItem *> getMethodsLines() {
        return methodsLines;
    }

    QList<CustomAttrText *>  getAttrs() {
        return attrs;
    }

    QList<QGraphicsLineItem *> getAttrsLines() {
        return attrsLines;
    }

    void popMethod() {
        if (methods.isEmpty()) {
            return;
        }
        CustomAttrText *tmp = methods.takeLast();
        delete tmp;
    }

    void popMethodsLine() {
        if (methodsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = methodsLines.takeLast();
        delete tmp;
    }

    void popAttr() {
        if (attrs.isEmpty()) {
            return;
        }
        CustomAttrText *tmp = attrs.takeLast();
        delete tmp;
    }

    void popAttrsLine() {
        if (attrsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = attrsLines.takeLast();
        delete tmp;
    }

    void pushAttr(CustomAttrText *item) {
        attrs.push_back(item);
    }

    void pushMethod(CustomAttrText *item) {
        methods.push_back(item);
    }

    void pushMethodLine(QGraphicsLineItem *item) {
        methodsLines.push_back(item);
    }

    void pushAttrLine(QGraphicsLineItem *item) {
        attrsLines.push_back(item);
    }

    qreal rowHeight() {
        return _rowHeight;
    }

    qreal rowWidth() {
        return _rowWidth;
    }

    qreal tabText() {
        return _tabText;
    }

    void moveLines(int st, long long el) {
        foreach (QGraphicsLineItem *val, methodsLines) {
            val->setPos(0, (el + st) * _rowHeight);
            st++;
        }
    }

    void moveTexts(int st, long long el) {
        foreach (CustomAttrText *val, methods) {
            val->setPos(0, (el + st) * _rowHeight + _tabText);
            st++;
        }
    }

    QGraphicsLineItem* createLine(qreal x, qreal y) {
        auto line = new QGraphicsLineItem(0, 0, _rowWidth, 0, this);
        line->setPos(x, y);
        return line;
    }

    QGraphicsTextItem* createText(qreal x, qreal y, QString text) {
        auto attr = new QGraphicsTextItem(text, this);
        setTextFlags(attr);
        attr->setPos(x, y);
        return attr;
    }

    QFlags<Qt::TextInteractionFlag> flags() {
        return _flags;
    }

    void setTextFlags(QGraphicsTextItem *item) {
        item->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditable     |
                                      Qt::TextInteractionFlag::TextSelectableByMouse |
                                      Qt::TextInteractionFlag::TextSelectableByKeyboard);
    }

private:
    QSet<ClassConnectionItem *> connections;
    QList<CustomAttrText *> attrs;
    QList<CustomAttrText *> methods;
    QList<QGraphicsLineItem *> attrsLines;
    QList<QGraphicsLineItem *> methodsLines;
    qreal _rowHeight;
    qreal _rowWidth;
    qreal _tabText;
    QFlags<Qt::TextInteractionFlag> _flags;
};

#endif // Object_H

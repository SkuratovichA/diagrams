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

#include "Connections/Connections.h"
#include "FillItems/FillItems.h"

QT_BEGIN_NAMESPACE
class QGraphicsItem;

class QGraphicsScene;

class QGraphicsSceneMouseEvent;

class QPointF;

QT_END_NAMESPACE

// for attrs and table expanding
class ClassTextAttr : public QGraphicsTextItem {
public:
    ClassTextAttr(ClassDiagramItem *p, QString text, qreal x, qreal y, QFlags<Qt::TextInteractionFlag> flags);
    ~ClassTextAttr();

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
        _rowHeight = 30.0;
        _rowWidth = width;
        _tabText = 2.0;
        _height = height;
        _width = width;
        _type = type;
        _boundingBox = QRectF(0.0, 0.0, _width, _height);
        _color = color;
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

    qreal height() const {
        return _height;
    }

    qreal width() const {
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
    qreal _rowHeight;
    qreal _rowWidth;
    qreal _tabText;
    qreal _height;
    qreal _width;
    QRectF _boundingBox;
    DiagramType _type;
    QColor _color;
};

class ActorLifetime : public QGraphicsLineItem {
public:
    ActorLifetime(QGraphicsItem *parent, QPointF startPoint);

    ~ActorLifetime();
};

class SequenceDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit SequenceDiagramItem(actorParams *params);

    void addConnection(ActorConnectionItem *connection);

    void removeConnection(ActorConnectionItem *connection);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QSet<ActorConnectionItem *> _connections;
    QGraphicsLineItem *line;
};

class ClassDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ClassDiagramItem(classParams *params);
    ~ClassDiagramItem();

    void addConnection(ClassConnectionItem *connection);

    void removeConnection(ClassConnectionItem *connection);

    QList<ClassTextAttr *> methods() {
        return _methods;
    }

    QList<QGraphicsLineItem *> methodsLines() {
        return _methodsLines;
    }

    QList<ClassTextAttr *> attrs() {
        return _attrs;
    }

    QList<QGraphicsLineItem *> attrsLines() {
        return _attrsLines;
    }

    void popMethod() {
        if (_methods.isEmpty()) {
            return;
        }
        ClassTextAttr *tmp = _methods.takeLast();
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
        ClassTextAttr *tmp = _attrs.takeLast();
        delete tmp;
    }

    void popAttrsLine() {
        if (_attrsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = _attrsLines.takeLast();
        delete tmp;
    }

    void pushAttr(ClassTextAttr *item) {
        _attrs.push_back(item);
    }

    void pushMethod(ClassTextAttr *item) {
        _methods.push_back(item);
    }

    void pushMethodLine(QGraphicsLineItem *item) {
        _methodsLines.push_back(item);
    }

    void pushAttrLine(QGraphicsLineItem *item) {
        _attrsLines.push_back(item);
    }

    void moveLines(int st, long long el) {
                foreach (QGraphicsLineItem *val, _methodsLines) {
                val->setPos(0, (el + st) * rowHeight());
                st++;
            }
    }

    void moveTexts(int st, long long el) {
                foreach (ClassTextAttr *val, _methods) {
                val->setPos(0, (el + st) * rowHeight() + tabText());
                st++;
            }
    }

    QGraphicsLineItem *createLine(qreal x, qreal y) {
        auto line = new QGraphicsLineItem(0, 0, rowWidth(), 0, this);
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

    qsizetype occupiedSockets() const {
        return _connections.count();
    }

    QPointF socket(uint32_t n) const {
        const auto margin = std::min(std::min(height()/2, width()/2), 10.0);
        const QPointF points[3] = {QPointF(0, margin), QPointF(-margin, -margin), QPointF(-margin, margin)};
        return centre() + points[n % 3];
    }

    QPointF topLeft() const {
        return pos();
    }

    QPointF topRight() const {
        return pos() + QPoint(width(), 0);
    }

    QPointF bottomLeft() const {
        return pos() + QPoint(0, height());
    }

    QPointF bottomRight() const {
        return pos() + QPoint(width(), height());
    }

    QPointF centre() const {
        return {x() + width() / 2.0, y() + height() / 2.0};
    }

    uint32_t numberOfSockets() const {
        return _numberOfSockets;
    }

    void setTextFlags(QGraphicsTextItem *item) {
        item->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditable |
                                      Qt::TextInteractionFlag::TextSelectableByMouse |
                                      Qt::TextInteractionFlag::TextSelectableByKeyboard);
    }

    QSet<ClassConnectionItem *> connections() const {
        return _connections;
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QList<ClassTextAttr *> _attrs;
    QList<ClassTextAttr *> _methods;
    QList<QGraphicsLineItem *> _attrsLines;
    QList<QGraphicsLineItem *> _methodsLines;
    QFlags<Qt::TextInteractionFlag> _flags;

    QSet<ClassConnectionItem *> _connections;
    uint32_t _numberOfSockets = 20;
};

#endif // Object_H

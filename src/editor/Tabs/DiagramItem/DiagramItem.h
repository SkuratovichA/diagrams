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

#include "../Connections/Connections.h"
#include "../FillItems/ObjectParams.h"

QT_BEGIN_NAMESPACE
class QGraphicsItem;

class QGraphicsScene;

class QGraphicsSceneMouseEvent;

class QPointF;

QT_END_NAMESPACE

class SequenceDiagramLifeLine;

// for attrs and table expanding
class ClassTextAttr : public QGraphicsTextItem {
public:
    ClassTextAttr(ClassDiagramItem *p, QString text, QPointF pos, QFlags<Qt::TextInteractionFlag> flags);
    ~ClassTextAttr() override;

    ClassDiagramItem *parent() {
        return _parent;
    }

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    ClassDiagramItem *_parent;
};

class NameObject : public QGraphicsTextItem {
public:
    NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, QPointF pos, QString str);

public:
    [[nodiscard]] QString name() const {
        return toPlainText();
    }

    void setName(const QString &name) {
        setPlainText(name.toStdString().c_str());
    }

    QGraphicsItem *parent() {
        return _parent;
    }

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

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

public:
    [[nodiscard]] virtual QString name() const = 0;
    [[nodiscard]] virtual QPointF centre() const = 0;

public:
    [[nodiscard]] qreal rowHeight() const {
        return _rowHeight;
    }

    void setName(const QString &name) const {
        _head->setName(name);
    }

    [[nodiscard]] qreal rowWidth() const {
        return _rowWidth;
    }

    [[nodiscard]] qreal tabText() const {
        return _tabText;
    }

    [[nodiscard]] DiagramType type() const {
        return _type;
    }

    [[nodiscard]] QRectF boundingBox() const {
        return _boundingBox;
    }

    void setBoundingBox(QRectF box) {
        _boundingBox = box;
    }

    void setBoundingBox(qreal a, qreal b, qreal c, qreal d) {
        _boundingBox = QRectF(a, b, c, d);
    }

    [[nodiscard]] QString typeStr() const {
        return type() == Actor ? QString("Actor") : QString("Class");
    }

    [[nodiscard]] qreal height() const {
        return _height;
    }

    [[nodiscard]] qreal width() const {
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

    void setColor(int r, int g, int b, int a) {
        _color = QColor(r, g, b, a);
    }

    void setColor(int r, int g, int b) {
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
//
//class ActorLifetime : public QGraphicsLineItem {
//public:
//    ActorLifetime(QGraphicsItem *parent, QPointF startPoint);
//};

class ClassDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ClassDiagramItem(classParams *params);
    ~ClassDiagramItem() override;

    void addConnection(ClassConnectionItem *connection);

    void removeConnection(ClassConnectionItem *connection);

public:
    [[nodiscard]] QString name() const override {
        return _head->toPlainText();
    }

    QPointF centre() const override {
        return {x() + width() / 2.0, y() + height() / 2.0};
    }

    [[nodiscard]] qsizetype occupiedSockets() const {
        return _connections.count();
    }

public:
    QList<ClassTextAttr *> methods() {
        return _methods;
    }

    [[nodiscard]] QList<QGraphicsLineItem *> methodsLines() const {
        return _methodsLines;
    }

    [[nodiscard]] QList<ClassTextAttr *> attrs() const {
        return _attrs;
    }

    [[nodiscard]] QList<QGraphicsLineItem *> attrsLines() const {
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
                val->setPos(0, static_cast<double>(el + st) * rowHeight());
                st++;
            }
    }

    void moveTexts(int st, long long el) {
                foreach (ClassTextAttr *val, _methods) {
                val->setPos(0, static_cast<double>(el + st) * rowHeight() + tabText());
                st++;
            }
    }

    QGraphicsLineItem *createLine(qreal x, qreal y) {
        auto line = new QGraphicsLineItem(0, 0, rowWidth(), 0, this);
        line->setPos(x, y);
        return line;
    }

    QGraphicsTextItem *createText(qreal x, qreal y, const QString &text) {
        auto attr = new QGraphicsTextItem(text, this);
        setTextFlags(attr);
        attr->setPos(x, y);
        return attr;
    }

    QFlags<Qt::TextInteractionFlag> flags() const {
        return _flags;
    }

    [[nodiscard]] QPointF socket(uint32_t n) const {
        const auto margin = std::min(std::min(height() / 2, width() / 2), 10.0);
        const QPointF points[3] = {QPointF(0, margin), QPointF(-margin, -margin), QPointF(-margin, margin)};
        return centre() + points[n % 3];
    }

    [[nodiscard]] QPointF topLeft() const {
        return pos();
    }

    [[nodiscard]] QPointF topRight() const {
        return pos() + QPoint(width(), 0);
    }

    [[nodiscard]] QPointF bottomLeft() const {
        return pos() + QPoint(0, height());
    }

    [[nodiscard]] QPointF bottomRight() const {
        return pos() + QPoint(width(), height());
    }

    static void setTextFlags(QGraphicsTextItem *item) {
        item->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditable |
                                      Qt::TextInteractionFlag::TextSelectableByMouse |
                                      Qt::TextInteractionFlag::TextSelectableByKeyboard);
    }

    [[nodiscard]] QSet<ClassConnectionItem *> connections() const {
        return _connections;
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QList<ClassTextAttr *> _attrs;
    QList<ClassTextAttr *> _methods;
    QList<QGraphicsLineItem *> _attrsLines;
    QList<QGraphicsLineItem *> _methodsLines;
    QFlags<Qt::TextInteractionFlag> _flags;

    QSet<ClassConnectionItem *> _connections;
};

class SequenceDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit SequenceDiagramItem(actorParams *params, ClassDiagramItem *parentClassDiagramItem_ = nullptr);

public:
    [[nodiscard]] QString name() const override {
        return _head->toPlainText();
    }

    [[nodiscard]] QPointF centre() const override {
        return {width() / 2.0, height()};
    }

    [[nodiscard]] qsizetype occupiedSockets() const {
        return _connections.count();
    }

public:
    void addConnection(SequenceConnectionItem *connection);
    void removeConnection(SequenceConnectionItem *connection);

    [[nodiscard]] ClassDiagramItem *parentClassDiagramItem() const {
        return _parentClassDiagramItem;
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    ClassDiagramItem *_parentClassDiagramItem = nullptr;
    QSet<SequenceConnectionItem *> _connections;
    SequenceDiagramLifeLine *_lifeLine = nullptr;
    qreal const lineDefaultLength = 500;
};

#endif // Object_H

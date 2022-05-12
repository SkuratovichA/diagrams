// File: DiagramItem.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

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

using namespace Connections;

QT_BEGIN_NAMESPACE
class QGraphicsItem;

class QGraphicsScene;

class QGraphicsSceneMouseEvent;

class QPointF;

QT_END_NAMESPACE

class SequenceDiagramLifeLine;

namespace CommandType {
    /**
     * @brief Type of a command. The enum is used for undo/redo commands.
     */
    enum CommandType {
        Add, Delete
    };

    /**
     * @brief Type of a request. Used in destruction commands. Can either be destruction by a connection or a self-destruction.
     */
    enum RequestType {
        Connection, Self
    };
};

/**
 * Self-expanding text class.
 */
class ClassTextAttr : public QGraphicsTextItem {
public:
    /**
     *
     * @param p
     * @param text
     * @param pos
     * @param flags
     */
    ClassTextAttr(ClassDiagramItem *p, QString text, QPointF pos, QFlags<Qt::TextInteractionFlag> flags);

    /**
     *
     */
    ~ClassTextAttr() override;

    /**
     *
     * @return
     */
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
        Actor, Class, Interface
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
        switch (type()) {
            case Actor:
                return QString("Actor");
            case Interface:
                return QString("Interface");
            case Class:
                return QString("Class");
        }
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

    void setRowWidth(qreal width) {
        _rowWidth = width;
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

    QFlags<Qt::TextInteractionFlag> myFlags() const {
        return _flags;
    }

    [[nodiscard]] qsizetype occupiedSockets() const {
        return _connections.count();
    }

    [[nodiscard]] QSet<ClassConnectionItem *> connections() const {
        return _connections;
    }

    static void setTextFlags(QGraphicsTextItem *item) {
        item->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditable |
                                      Qt::TextInteractionFlag::TextSelectableByMouse |
                                      Qt::TextInteractionFlag::TextSelectableByKeyboard);
    }

    QList<ClassTextAttr *> &methods() {
        return _methods;
    }

    [[nodiscard]] QList<QGraphicsLineItem *> methodsLines() const {
        return _methodsLines;
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

    void moveLines(int st, long long el) {
        for (QGraphicsLineItem *val : _methodsLines) {
            val->setPos(0, static_cast<double>(el + st) * rowHeight());
            st++;
        }
    }

    void moveTexts(int st, long long el) {
        for (ClassTextAttr *val : _methods) {
            val->setPos(0, static_cast<double>(el + st) * rowHeight() + tabText());
            st++;
        }
    }

    void pushMethod(ClassTextAttr *item) {
        _methods.push_back(item);
    }

    void pushMethodLine(QGraphicsLineItem *item) {
        _methodsLines.push_back(item);
    }


    NameObject *_head;
    QList<QGraphicsLineItem *> _methodsLines;
    QList<ClassTextAttr *> _methods;
    QFlags<Qt::TextInteractionFlag> _flags;
    QSet<ClassConnectionItem *> _connections;
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

class ClassInterfaceItem : public QGraphicsRectItem, public DiagramItem {
public:
    explicit ClassInterfaceItem();
};

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

    void setDeleted(bool isDeleted) {
        _isDeleted = isDeleted;
    }

    bool isDeleted() const {
        return _isDeleted;
    }

public:
    [[nodiscard]] QList<ClassTextAttr *> attrs() const {
        return _attrs;
    }

    [[nodiscard]] QList<QGraphicsLineItem *> attrsLines() const {
        return _attrsLines;
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

    void pushAttrLine(QGraphicsLineItem *item) {
        _attrsLines.push_back(item);
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

    [[nodiscard]] QPointF socket(uint32_t n) const {
        const auto margin = std::min(std::min(height() / 2, width() / 2), 30.0);
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

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QList<ClassTextAttr *> _attrs;
    QList<QGraphicsLineItem *> _attrsLines;

    bool _isDeleted = false; ///< if deleted. Affects SequenceDiagramItem.
};

/**
 *
 */
class SequenceDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    /**
     * @brief A constructor.
     * @param params parameters needed to create an object.
     * @param parentClassDiagramItem_ parent class from class diagram item.
     */
    explicit SequenceDiagramItem(SequenceDiagramItemParameters *params,
                                 ClassDiagramItem *parentClassDiagramItem_ = nullptr);

    /**
     * Getterrs, setters
     */
public:
    /**
     * @brief Name of an object.
     * @return Name of a head of an object.
     */
    [[nodiscard]] QString name() const override {
        return _head->toPlainText();
    }

    const QSet<SequenceConnectionItem *> &getRemovedConnectionsOnDeleteSelf() const {
        return _removedConnectionsOnDeleteSelf;
    }

    const QSet<SequenceConnectionItem *> &getRemovedConnectionsOnDeleteConnection() const {
        return _removedConnectionsOnDeleteConnection;
    }

    const QSet<SequenceConnectionItem *> &getRemovedConnectionsOnAddSelf() const {
        return _removedConnectionsOnAddSelf;
    }

    const QSet<SequenceConnectionItem *> &getRemovedConnectionsOnAddConnection() const {
        return _removedConnectionsOnAddConnection;
    }

    /**
     * @brief Local centre.
     * @return local centre of an object.
     */
    [[nodiscard]] QPointF localCentre() const {
        return {width() / 2.0, height()};
    }

    /**
     * @brief Getter.
     * @return set with connections
     */
    QSet<SequenceConnectionItem *> connections() const {
        return _connections;
    };

    /**
     * @brief Get all connection, both deleted and existent.
     * @return Set with all connections.
     */
    QSet<SequenceConnectionItem *> allConnections() const {
        QSet<SequenceConnectionItem *> allConnections = QSet<SequenceConnectionItem *>();
        allConnections.unite(_connections);
        allConnections.unite(_removedConnectionsOnAddConnection);
        allConnections.unite(_removedConnectionsOnAddSelf);
        allConnections.unite(_removedConnectionsOnDeleteConnection);
        allConnections.unite(_removedConnectionsOnDeleteSelf);
        return allConnections;
    }

    /**
     * @brief Functions tracks all changes and notifies children.
     */
    void trackNodes();

    /**
     * @brief The centre of an object. (the middle point on the bottoom edge of the box).
     * @return centre point for this type of object.
     */
    [[nodiscard]] QPointF centre() const override {
        return {x() + width() / 2.0, y() + height()};
    }

public:
    /**
     * @brief Add a connection to the set of connections for a certain item.
     * @param connection Connection object.
     * @param actorType Type of an actor. Can either be receiver or a sender.
     * @param commandType Connection can be added on undo delete, create (redo) commands. There is a need to differ this.
     * @param requestType Type of a request. Request can either be from a Connection object or from a self. (self-destruction).
     */
    void addConnection(SequenceConnectionItem *connection,
                       ActorType actorType,
                       CommandType::CommandType commandType,
                       CommandType::RequestType requestType);

    /**
     * @brief Remove a connection to the set of connections for certain item.
     * @param connection connection to remove.
     * @param commandType Connection can be deleted on delete (+redo), undo create commands. There is a need to differ this.
     * @param requestType  Type of a request. Request can either be from a Connection object or from a self. (self-creation).
     */
    void removeConnection(SequenceConnectionItem *connection,
                          CommandType::CommandType commandType,
                          CommandType::RequestType requestType);

    /**
     * @brief Parent getter.
     * @return parent from class diagram.
     */
    [[nodiscard]] ClassDiagramItem *parentClassDiagramItem() const {
        return _parentClassDiagramItem;
    }

    /**
     * @brief Parent setter.
     * @param parent from class diagram to connect with the instance of a sequence diagram.
     */
    void setParent(ClassDiagramItem *parent) {
        _parentClassDiagramItem = parent;
    }

    /**
     * @brief _lineLength getter.
     * @return
     */
    qreal lineLength() const {
        return _lineLength;
    }

    /**
     * Overriden functions.
     */
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * Constants
     */
private:
    ClassDiagramItem *_parentClassDiagramItem = nullptr; ///< pointer to the "parent" class diagram item for synchronization.
    SequenceDiagramLifeLine *_lifeLine = nullptr; ///< lifeLine to track active regions
    qreal const _lineLength = 1000; ///< default length of a life line

    QSet<SequenceConnectionItem *> _connections = QSet<SequenceConnectionItem *>(); ///< connections to track
    QSet<SequenceConnectionItem *> _removedConnectionsOnDeleteSelf = QSet<SequenceConnectionItem *>(); ///< see the name
    QSet<SequenceConnectionItem *> _removedConnectionsOnDeleteConnection = QSet<SequenceConnectionItem *>(); ///< see the name
    QSet<SequenceConnectionItem *> _removedConnectionsOnAddSelf = QSet<SequenceConnectionItem *>(); ///< see the name
    QSet<SequenceConnectionItem *> _removedConnectionsOnAddConnection = QSet<SequenceConnectionItem *>(); ///< see the name
};

#endif // Object_H
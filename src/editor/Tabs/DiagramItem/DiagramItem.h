/** @file DiagramItem.h
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @author Shchapaniak Andrei <xshcha00\@vutbr.cz>
 * @date 07.05.2022
 */


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

/**
 * Type of the command. Used in undo/redo commands
 */
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
 * @brief Self-expanding text class.
 */
class ClassTextAttr : public QGraphicsTextItem {
public:

    /**
     * @brief A constructor.
     * @details This constructor creates a text connected with its parent.
     * @param p parent item
     * @param text text represented by a string
     * @param pos position of the text
     * @param flags flags for text that make it editable
     */
    ClassTextAttr(ClassDiagramItem *p, QString text, QPointF pos, QFlags<Qt::TextInteractionFlag> flags);

    /**
     * @brief A destructor.
     */
    ~ClassTextAttr() override;

    /*
     * @brief Getter. Get a paent object
     */
    ClassDiagramItem *parent() {
        return _parent;
    }

protected:
    /**
     * @brief Handles an event of the text editing for object attributes to align it.
     * @details In the case of entering "Key_Enter" the text editing is ended.
     * @param event key event
     */
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    ClassDiagramItem *_parent; ///< parent object of class ClassDiagramItem
};

/**
 * @brief Name above the class object
 */
class NameObject : public QGraphicsTextItem {
public:
    /**
     * @brief A constructor.
     * @details This constructor creates a text connected with its parent.
     * @param parent parent item
     * @param flags flags for text that make it editable
     * @param pos position of the text
     * @param str text represented by a string
     */
    NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, QPointF pos, QString str);

public:
    /**
     * Getter. Get a text representation of a name
     * @return QString
     */
    [[nodiscard]] QString name() const {
        return toPlainText();
    }

    /**
     * Setter. Set the name
     * @param name QString name
     */
    void setName(const QString &name) {
        setPlainText(name.toStdString().c_str());
    }

    /**
     * Getter. Get a parent
     * @return
     */
    QGraphicsItem *parent() {
        return _parent;
    }

protected:
    /**
     * @brief Handle an event of the text editing for object name to align it.
     * @details In the case of entering "Key_Enter" the text editing is ended.
     * @param event key event
     */
    void keyReleaseEvent(QKeyEvent *event) override;

    QGraphicsItem *_parent; ///< a pointer on the parent object
};

/**
 * @brief An abstract class of a diagram item, which represents.
 */
class DiagramItem {
public:
    /**
     * There are two types of a diagram item: actor from the sequence diagram, and class from
     */
    enum DiagramType {
        Actor, Class, Interface
    };

    /**
     * @brief Create a default diagram item
     * @param width initial width
     * @param height initial height
     * @param type diagram type
     * @param color initial color
     */
    explicit DiagramItem(qreal width, qreal height, QColor color, QString type) {
        qDebug() << width << height;
        _rowHeight = 30.0;
        _rowWidth = width;
        _textMargin = 2.0;
        _height = height;
        _width = width;
        _type = type;
        _boundingBox = QRectF(0.0, 0.0, _width, _height);
        _color = color;
    }

public:
    /**
     * @brief Pure virtual method for an object name
     * @return QString name
     */
    [[nodiscard]] virtual QString name() const = 0;

    /**
     * @brief Pure virtual method for a centre of an object
     * @return QPointF centre
     */
    [[nodiscard]] virtual QPointF centre() const = 0;

public:
    /**
     * @brief Getter. Row height.
     * @return qreal
     */
    [[nodiscard]] qreal rowHeight() const {
        return _rowHeight;
    }

    /**
     * @brief Setter. Set a name
     * @param name A new name
     */
    void setName(const QString &name) const {
        _head->setName(name);
    }

    /**
     * @brief Getter. Get row width
     * @return qreal _rowWidth
     */
    [[nodiscard]] qreal rowWidth() const {
        return _rowWidth;
    }

    /**
     * @brief Getter. Get a tab text position
     * @return qreal _textMargin
     */
    [[nodiscard]] qreal tabText() const {
        return _textMargin;
    }

    /**
     * @brief Getter. Get a diagram type
     * @return DiagramType. One of the Actor, Class
     */
    [[nodiscard]] QString myType() const {
        return _type;
    }

    /**
     * @brief Getter. Get a bounding box
     * @return QRectF bounding box
     */
    [[nodiscard]] QRectF boundingBox() const {
        return _boundingBox;
    }

    /**
     * @brief Getter. Get object height.
     * @return qreal _height
     */
    void setBoundingBox(QRectF box) {
        _boundingBox = box;
    }

    void setBoundingBox(qreal a, qreal b, qreal c, qreal d) {
        _boundingBox = QRectF(a, b, c, d);
    }

    [[nodiscard]] qreal height() const {
        return _height;
    }

    /**
     * @brief Getter. Get object width
     * @return qreal _width
     */
    [[nodiscard]] qreal width() const {
        return _width;
    }

    /**
     * @brief Setter. Set width
     * @param width New width
     */
    void setWidth(qreal width) {
        _width = width;
    }

    /**
     * @brief Setter. Set row width
     * @param width qreal width
     */
    void setRowWidth(qreal width) {
        _rowWidth = width;
    }

    /**
     * @brief Setter. Set height of an object
     * @param height New height
     */
    void setHeight(qreal height) {
        _height = height;
    }

    /**
     * @brief Getter. Return color of an object
     * @return QColor _color
     */
    QColor color() {
        return _color;
    }

public:
    NameObject *_head; ///< head of the object, which is its name

private:
    qreal _rowHeight; ///< height of an object's row
    qreal _rowWidth; ///< width of an object's row
    qreal _textMargin; ///<
    qreal _height; ///< height of the object
    qreal _width; ///< width of the object
    QRectF _boundingBox; ///< bounding box for an object
    QString _type; ///< type of an object.
    QColor _color; ///< color of an object
};

/**
 * @brief One of the two main object in the class diagram editor.
 */
class ClassDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    /**
     * @brief A constructor.
     * This constructor creates an entity for the class diagram.
     * @param params class with all necessary information to create an object.
     */
    explicit ClassDiagramItem(ClassDiagramItemParameters *params);

    /**
     * @brief A destructor.
     */
    ~ClassDiagramItem() override;

    /**
     * @brief Add a connection to the set of connections for certain item.
     * @param connection relation arrow between objects
     */
    void addConnection(ClassConnectionItem *connection);

    /**
     * @brief Remove a connection to the set of connections for certain item.
     * @param connection relation arrow between objects
     */
    void removeConnection(ClassConnectionItem *connection);

public:
    /**
     * @brief Implements name() function from DiagramItem class
     * @return The name of the class
     */
    [[nodiscard]] QString name() const override {
        return _head->toPlainText();
    }

    /**
     * @brief Implement centre() function from DiagramItem class
     * @return The centre of the object
     */
    QPointF centre() const override {
        //qDebug() << width() << height();
        return {x() + width() / 2.0, y() + height() / 2.0};
    }

    /**
     * @brief Getter. Number of occupied sockets
     * @return number of connections the class already has
     */
    [[nodiscard]] qsizetype occupiedSockets() const {
        return _connections.count();
    }

    /**
     * @brief Setter. Remove or restore an object. Used in sequence diagram checks
     * @param isDeleted New valueither Actor or Class
     */
    void setDeleted(bool isDeleted) {
        _isDeleted = isDeleted;
    }

    /**
     * @brief Getter. If object is deleted, retrn true
     * @return boolean
     */
    bool isDeleted() const {
        return _isDeleted;
    }

public:
    /**
     * @brief Getter. Returns methods of an object
     * @return QList<ClassTextAttr *> all methods
     */
    QList<ClassTextAttr *> &methods() {
        return _methods;
    }

    /**
     * @brief Getter. Returns method lines
     * @return QList<QGraphicsLineItem *>
     */
    [[nodiscard]] QList<QGraphicsLineItem *> methodsLines() const {
        return _methodsLines;
    }

    /**
     * @brief Getter. Returns attribudet of a class object
     * @return QList<ClassTextAttr *> a list with all attributes
     */
    [[nodiscard]] QList<ClassTextAttr *> attrs() const {
        return _attrs;
    }

    /**
     * @brief Getter. Returns lines for attibutes
     * @return QList<QGraphicsLineItem *> lines for attributes
     */
    [[nodiscard]] QList<QGraphicsLineItem *> attrsLines() const {
        return _attrsLines;
    }

    /**
     * @brief Delete a class method
     */
    void popMethod() {
        if (_methods.isEmpty()) {
            return;
        }
        ClassTextAttr *tmp = _methods.takeLast();
        delete tmp;
    }

    /**
     * @brief Delete a method line
     */
    void popMethodsLine() {
        if (_methodsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = _methodsLines.takeLast();
        delete tmp;
    }

    /**
     * @brief Delete an attribute
     */
    void popAttr() {
        if (_attrs.isEmpty()) {
            return;
        }
        ClassTextAttr *tmp = _attrs.takeLast();
        delete tmp;
    }

    /**
     * @brief Delete an attribute line
     */
    void popAttrsLine() {
        if (_attrsLines.isEmpty()) {
            return;
        }
        QGraphicsLineItem *tmp = _attrsLines.takeLast();
        delete tmp;
    }

    /**
     * @brief Add an attribute to class attributes
     * @param item ClassTextAttr* a new attribute
     */
    void pushAttr(ClassTextAttr *item) {
        _attrs.push_back(item);
    }

    /**
     * @brief Add a method to class methods
     * @param item ClassTextAttr* a new method
     */
    void pushMethod(ClassTextAttr *item) {
        _methods.push_back(item);
    }

    /**
     * @brief Add a breakline method line
     * @param item QGraphicsLineItem * a new breakline
     */
    void pushMethodLine(QGraphicsLineItem *item) {
        _methodsLines.push_back(item);
    }

    /**
     * @brief Add a breakline for class attributes
     * @param item QGraphicsLineItem * a breakline
     */
    void pushAttrLine(QGraphicsLineItem *item) {
        _attrsLines.push_back(item);
    }

     /**
      * @brief Move all lines when new attribute or method is added
      * @param startPosition start position
      * @param numberOfAttributes number of attributes
      */
    void moveLines(int startPosition, long long numberOfAttributes) {
        for (QGraphicsLineItem *val: _methodsLines) {
            val->setPos(0, static_cast<double>(numberOfAttributes + startPosition) * rowHeight());
            startPosition++;
        }
    }

    /**
     * @brief Move all text attributes (both class methods and attributes)
     * @param startPosition
     * @param numberOfAttributes
     */
    void moveTexts(int startPosition, long long numberOfAttributes) {
        for (ClassTextAttr *val: _methods) {
            val->setPos(0, static_cast<double>(numberOfAttributes + startPosition) * rowHeight() + tabText());
            startPosition++;
        }
    }

    /**
     * @brief Cerates a breakline. Used when adding a method or an attribute to the table
     * @param x axis
     * @param y axis
     * @return Created line
     */
    QGraphicsLineItem *createLine(qreal x, qreal y) {
        auto line = new QGraphicsLineItem(0, 0, rowWidth(), 0, this);
        line->setPos(x, y);
        return line;
    }

    /**
     * @brief Getter. Get flags for text
     * @return QFlags<Qt::TextInteractionFlag> flags
     */
    QFlags<Qt::TextInteractionFlag> flags() const {
        return _flags;
    }

    /**
     * @brief Get a socket given the order
     * @details
     * The object contains three points in the centre in the shape of a rectangle.
     * This method retuans one of the three points, when line calls it.
     * Hence, the line will know its position.
     * @param n
     * @return QPointF centre of che object
     */
    [[nodiscard]] QPointF socket(uint32_t n) const {
        if (occupiedSockets() == 1) {
            return centre();
        }

        const auto margin = std::min(std::min(height() / 2, width() / 2), 30.0);
        //qDebug() << margin << width() << height();
        const QPointF points[3] = {QPointF(0, margin), QPointF(-margin, -margin), QPointF(-margin, margin)};
        return centre() + points[n % 3];
    }

    /**
     * @brief Top left point
     * @return QPointF
     */
    [[nodiscard]] QPointF topLeft() const {
        return pos();
    }

    /**
     * @brief Top right point
     * @return QPointF
     */
    [[nodiscard]] QPointF topRight() const {
        return pos() + QPoint(width(), 0);
    }

    /**
     * @brief Bottom left point
     * @return QPointF
     */
    [[nodiscard]] QPointF bottomLeft() const {
        return pos() + QPoint(0, height());
    }

    /**
     * @brief Bottom right point
     * @return QPointF
     */
    [[nodiscard]] QPointF bottomRight() const {
        return pos() + QPoint(width(), height());
    }

    /**
     * @brief Set flags for text manipulations
     * @param item text item
     */
    static void setTextFlags(QGraphicsTextItem *item) {
        item->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditable |
                                      Qt::TextInteractionFlag::TextSelectableByMouse |
                                      Qt::TextInteractionFlag::TextSelectableByKeyboard);
    }

    /**
     * @brief Getter. Return connections
     * @return QSet<ClassConnectionItem *>  all connections
     */
    [[nodiscard]] QSet<ClassConnectionItem *> connections() const {
        return _connections;
    }

    /**
     * Overriden function. Mouse press event
     * @param event an event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    /**
     * Overriden function. Tracks changing of the object's position
     * @param change change
     * @param value values
     * @return QVariant
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QList<ClassTextAttr *> _attrs; ///< class attributes
    QList<ClassTextAttr *> _methods; ///< class methods
    QList<QGraphicsLineItem *> _attrsLines; ///< lines separating the class attributes
    QList<QGraphicsLineItem *> _methodsLines; ///< lines separating the class methods
    QFlags<Qt::TextInteractionFlag> _flags; ///< flags for text manipulation

    QSet<ClassConnectionItem *> _connections; ///< class connections

    bool _isDeleted = false; ///< if deleted. Affects SequenceDiagramItem.
};

/**
 * @brief One of two main object in sequence diagram.
 * Inherits from QGraphicsRectItem and DiagramItem
 */
class SequenceDiagramItem : public QGraphicsRectItem, public DiagramItem {
public:
    /**
     * @brief A constructor.
     * @details
     * This constructor creates an entity for the sequence diagram.
     * It is a simple rectangle with a child item, which represents
     * the name of the object.
     *
     * @param params class with all necessary information to create an object.
     */
    explicit SequenceDiagramItem(SequenceDiagramItemParameters *params,
                                 ClassDiagramItem *parentClassDiagramItem_ = nullptr);

public:
    /**
     * @brief Name of an object.
     * @return Name of a head of an object.
     */
    [[nodiscard]] QString name() const override {
        return _head->toPlainText();
    }

    /**
     * @brief Getter. Get connections, that were deleted by an object itself on delete command
     * @return connections
     */
    const QSet<SequenceConnectionItem *> &getRemovedConnectionsOnDeleteSelf() const {
        return _removedConnectionsOnDeleteSelf;
    }

    /**
     * @brief Getter. Get connections, that were removed by a connection on delete comand
     * @return connections
     */
    const QSet<SequenceConnectionItem *> &getRemovedConnectionsOnDeleteConnection() const {
        return _removedConnectionsOnDeleteConnection;
    }

    /**
     * @brief Getter. Get connections, that were removed by an object itself on undo add command
     * @return connections
     */
    const QSet<SequenceConnectionItem *> &getRemovedConnectionsOnAddSelf() const {
        return _removedConnectionsOnAddSelf;
    }

    /**
     * Get connection, that were removed by a connection on undo add command
     * @return
     */
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

protected:
    /**
     * Notify custom items that some part of the item's state changes.
     * Reimplementation of this function provides a possibility to move
     * an item only on x axis.
     *
     * @param change what was changed
     * @param value new value of changed item
     *
     * @return new value
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

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

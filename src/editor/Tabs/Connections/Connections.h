/** @file Connections.h
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 02.05.2022
 */


#ifndef DIAGRAMS_CONNECTIONS_H
#define DIAGRAMS_CONNECTIONS_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include "../TabCanvas/ItemsBuffer/Itemsbuffer.h"

class ClassDiagramItem;

class SequenceDiagramItem;

/**
 * @brief A class for a self-exppainding text item 
 */
class msgText : public QGraphicsTextItem {
public:
    /**
     * @brief A Constructor.
     * @param parent Parent QGraphicsItem object.
     * @param flags Text flags
     * @param x initial x postion
     * @param y initial y position
     * @param str default string
     */
    msgText(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y, QString str);

    /**
     * @brief Getter.
     * @return parent which this class is attached to.
     */
    QGraphicsItem *parent() {
        return _parent;
    }

protected:
    /**
     * @brief overrriden method reacting to the key release event.
     * @param event
     */
    void keyReleaseEvent(QKeyEvent *event) override;

    /**
     * Set the text and change color, if needed after focusing out of the text
     * @param event
     */
    void focusOutEvent(QFocusEvent *event) override;

private:
    QGraphicsItem *_parent; ///< an object the text belogs to
};

/**
 * @brief Connection between two classes in the class diagram.
 * @details
 * There are several supported types of connectinos:
 * - Aggregation
 * - Composition
 * - Generalization
 * - Association
 * - Dependency
 *
 * In the case of generalization, "overriden" methods are marked with cyan color.
 * Connection can have a message above the connection line, and an arrow depending on its type.
 *
 */
class ClassConnectionItem : public QGraphicsLineItem {
public:
    /**
     * Type of a connectino
     */
    enum ClassConnectionType {
        Aggregation, Composition, Generalization, Association, Dependency
    };

    /**
     *
     * @param fromNode Initial node
     * @param toNode Final node
     * @param params Parameters of a connection
     * @param type Type of a connection
     * @param order Order of a connection used by a connectino to get a socket
     * @param color Connection color (default: black)
     */
    ClassConnectionItem(
            ClassDiagramItem *fromNode,
            ClassDiagramItem *toNode,
            relationsParams *params,
            ClassConnectionType type,
            uint32_t order = 0,
            QColor color = QColor(50, 45, 50, 100)
    );

    /**
     * @brief A destructor.
     */
    ~ClassConnectionItem();

public:
    /**
     * @brief Connection type setter.
     * @param type A new type
     */
    void setType(ClassConnectionType type) {
        _connectionType = type;
    }

    /**
     * @brief Connection type getter.
     * @return Connection type
     */
    [[nodiscard]] ClassConnectionType connectionType() const {
        return _connectionType;
    }

    /**
     * @brief Node "from" getter
     * @return initial node connection connects to
     */
    [[nodiscard]] ClassDiagramItem *nodeFrom() const {
        return _nodeFrom;
    }

    /**
     * @brief Node "to" getter
     * @return final node connection connects to
     */
    [[nodiscard]] ClassDiagramItem *nodeTo() const {
        return _nodeTo;
    }

    /**
     * @brief Connection color getter.
     * @return connection color
     */
    [[nodiscard]] QColor color() const {
        return _color;
    }

    /**
     * @brief Connection order
     * @return integer number which is an order number of a connection
     */
    [[nodiscard]] uint32_t order() const {
        return _order;
    }

    /**
     * @brief Change connection orientation
     */
    void changeOrientation() {
        _orientation = !_orientation;
        trackNodes();
    }

    /**
     * @brief Getter. Get a left number (belongs to nodeFrom)
     * @return msgText class
     */
    msgText *getLeftNum() const {
        return leftNum;
    }

    /**
     * @brief Getter. Get a right number (belongs to nodeTo)
     * @return msgText class
     */
    msgText *getRightNum() const {
        return rightNum;
    }

    /**
     * @brief Getter. Get a message above the connection line
     * @return msgText class with a message above the connection
     */
    msgText *getMsg() const {
        return msg;
    }

    /**
     * @brief Set position of the text near the ending point of the connection line
     * @param pos QPointF() position
     */
    void leftSetPos(QPointF pos) {
        leftNum->setPos(pos);
    }

    /**
     * @brief Set position of the text of the starting point of the connection line
     * @param pos
     */
    void rightSetPos(QPointF pos) {
        rightNum->setPos(pos);
    }

    /**
     * @brief Set position of the message on the top of the connection
     */
    void msgSetPos(QPointF pos) {
        msg->setPos(pos);
    }

    /**
     * @brief Setter. Set a new color for the connectino
     * @param color A new color
     */
    void setColor(const QColor &color) {
        _color = color;
    }

    /**
     * Getter. Get flags for the text messages.
     * @return Qt::TextInterationFlag
     */
    static QFlags<Qt::TextInteractionFlag> getFlags() {
        return Qt::TextInteractionFlag::TextEditable |
               Qt::TextInteractionFlag::TextSelectableByMouse |
               Qt::TextInteractionFlag::TextSelectableByKeyboard;
    }

    /**
     * Getter. Connection is of a "single" type if it nodeFrom == nodeTo
     * @return boolean value
     */
    bool single() {
        return _single;
    }

public:
    /**
     * Function for updating the line depengin on the position of the nodes
     */
    void trackNodes();

protected:
    /**
     * Track mouse press event
     * @param event event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    /**
     * @brief Draw a line.
     * @param painter
     * @param option
     */
    void drawLine(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /**
     * @brief Get a bounding box for a connection line.
     * @return
     */
    [[nodiscard]] QPolygonF lineShaper() const;

    /**
     * @brief Get edge points lying on the edges of the boxes (clasess)
     * @details
     * if a point is not placed to the centre of the box
     * compute intersections like there was #2 octant.
     * Basically, the idea is the same for all the octants.
     * If one octant cannot find the intersections. E.g. line is found using the formula,
     * the intersection point does not lie on the edge we want ->
     * use the same formula for finding an intersection point for n+1 octant
     * @return QPair<QPointF, QPointF> a pair with two points
     */
    [[nodiscard]] QPair<QPointF, QPointF> edgePoints() const;

    /**
     * @brief Set a bounting box for a connection line
     * @return
     */
    [[nodiscard]] QPainterPath shape() const override;

    /**
     * @bief Draw a line. Overriden methods.
     * @param painter A painter
     * @param option An option
     * @param widget A widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool _single;

private:
    ClassDiagramItem *_nodeFrom; ///< the first node with the start of the line
    ClassDiagramItem *_nodeTo; ///< the second node with the end of the line
    ClassConnectionType _connectionType; ///< A type of the connection
    bool _orientation = true; ///< orientation (from->to or to->from)
    QColor _color; ///< line color
    uint32_t _order; ///< line order to get a centre of a class box
    msgText *msg; ///< message above the line
    msgText *leftNum; ///< message near nodeFrom
    msgText *rightNum; ///< message near nodeTo
};

/**
 * Namespace with a ConnectionType and ActorType for a sequence connection
 */
namespace Connections {
    /**
     * Connection type.
     */
    enum ConnectionType {
        Synchronous,
        Asynchronous,
        Reply,
        Create,
        Delete,
        Undefined,
    };

    /**
     * Actor type
     */
    enum ActorType {
        Caller,
        Receiver
    };
};
using namespace Connections;

/**
 * @brief Sequence connection used in the sequence diagram.
 * @details
 * Line connects two lifelines of two actors.
 * Line cap be one of ConnectionType
 */
class SequenceConnectionItem : public QGraphicsLineItem {
public:
    /**
     * @brief A constructor.
     * @param nodeFrom A starting point
     * @param nodeTo An ending point
     * @param params Connection parameters
     * @param connectionType A Connection type
     */
    SequenceConnectionItem(
            SequenceDiagramItem *nodeFrom,
            SequenceDiagramItem *nodeTo,
            messageParams *params,
            ConnectionType connectionType
    );

    /**
     * @brief A destructor
     */
    ~SequenceConnectionItem();

public:

    /**
     * @brief Tracking nodes and changing the x position of a start and endpoint
     */
    void trackNodes();

    /**
     * @brief Getter. Returns a type of a connection.
     * @return ConnectionType
     */
    [[nodiscard]] ConnectionType connectionType() const {
        return _connectionType;
    };

    /**
     * @brief Getter. Returns a nodeFrom (initial node)
     * @return SequenceDiagramItem nodeFrom
     */
    [[nodiscard]] SequenceDiagramItem *nodeFrom() const {
        return _nodeFrom;
    };

    /**
     * @brief Getter. Returns a nodeTo (final node)
     * @return SequenceDiagramItem nodeTo
     */
    [[nodiscard]] SequenceDiagramItem *nodeTo() const {
        return _nodeTo;
    };

    /**
     * @brief Getter. Returns the type of a connection.
     * @return ConnectionType
     */
    ConnectionType type() {
        return _connectionType;
    }

    /**
     * Getter. Returns flags for a text above the connection line
     * @return Qt::TextInteractionFlags
     */
    static QFlags<Qt::TextInteractionFlag> getFlags() {
        return Qt::TextInteractionFlag::TextEditable |
               Qt::TextInteractionFlag::TextSelectableByMouse |
               Qt::TextInteractionFlag::TextSelectableByKeyboard;
    }

    /**
     * Returns width for a text above the connection line
     * @return qreal
     */
    qreal widthText() {
        return text->boundingRect().width();
    }

    /**
     * Getter. Returns text above a connection line
     * @return QGraphicsTextItem*
     */
    QGraphicsTextItem* getText() {
        return text;
    };

    /**
     * @brief If linetype is reply, set it in an appropriate way
     */
    void setLineReplay() {
        cLine = line();

        if (cLine.p1().x() < cLine.p2().x()) {
            cLine = QLineF(QPointF(cLine.p1().x() + 10, cLine.p1().y()),
                           QPointF(cLine.p2().x() - 10, cLine.p2().y()));

            arrowTopPoint = QPointF(cLine.p1().x() + 15, cLine.p1().y() + 8);
            arrowBottomPoint = QPointF(cLine.p1().x() + 15, cLine.p1().y() - 8);
            lineEndpoint = cLine.p1();
        }
        else {
            cLine = QLineF(QPointF(cLine.p1().x() - 10, cLine.p1().y()),
                           QPointF(cLine.p2().x() + 10, cLine.p2().y()));

            arrowTopPoint = QPointF(cLine.p1().x() - 15, cLine.p1().y() + 8);
            arrowBottomPoint = QPointF(cLine.p1().x() - 15, cLine.p1().y() - 8);
            lineEndpoint = cLine.p1();
        }
    }

    /**
     * @brief Configure another lines (Reply type excluded)
     */
    void setLineNonReply() {
        cLine = line();

        if (cLine.p1().x() < cLine.p2().x()) {
            cLine = QLineF(QPointF(cLine.p1().x() + 10, cLine.p1().y()),
                           QPointF(cLine.p2().x() - 10, cLine.p2().y()));

            arrowTopPoint = QPointF(cLine.p2().x() - 15, cLine.p2().y() + 8);
            arrowBottomPoint = QPointF(cLine.p2().x() - 15, cLine.p2().y() - 8);
            lineEndpoint = cLine.p2();
            lineEndPointX = lineEndpoint.x();
        }
        else {
            cLine = QLineF(QPointF(cLine.p1().x() - 10, cLine.p1().y()),
                           QPointF(cLine.p2().x() + 10, cLine.p2().y()));

            arrowTopPoint = QPointF(cLine.p2().x() + 15, cLine.p2().y() + 8);
            arrowBottomPoint = QPointF(cLine.p2().x() + 15, cLine.p2().y() - 8);
            lineEndpoint = cLine.p2();
            lineEndPointX = lineEndpoint.x() - 20;
        }
    }

public:
    QRectF lineShaper() const;
    QPainterPath shape() const override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void paintSynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintAsynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintReply(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintCreate(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintDelete(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPointF lineEndpoint; ///< line endpoint (p2)
    QPointF arrowTopPoint; ///< top point of the line arrow
    QPointF arrowBottomPoint; ///< bottom point of the line arrow
    QLineF cLine; ///< a line to draw
    qreal lineEndPointX; ///< x axis of an endpoint of the line
    qreal arrowSizeScale = 20; ///< size of an arrow
    msgText *text; ///< text above the line
    QColor _color = QColor(0, 0, 0, 200); ///< default color
    ConnectionType _connectionType; ///< connection Type
    SequenceDiagramItem *_nodeTo; ///< start node
    SequenceDiagramItem *_nodeFrom; ///< end node
};

#endif //DIAGRAMS_CONNECTIONS_H

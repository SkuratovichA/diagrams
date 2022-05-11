// File: Connections.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#ifndef DIAGRAMS_CONNECTIONS_H
#define DIAGRAMS_CONNECTIONS_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include "../TabCanvas/ItemsBuffer/Itemsbuffer.h"

class ClassDiagramItem;

class SequenceDiagramItem;

class msgText : public QGraphicsTextItem {
public:
    msgText(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y, QString str);
    //~msgText();

    QGraphicsItem *parent() {
        return _parent;
    }

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QGraphicsItem *_parent;
};

/**
 *
 */
class ClassConnectionItem : public QGraphicsLineItem {
    /** Constructor, destructor
     */
public:
    enum ClassConnectionType {
        Aggregation, Composition, Generalization, Association, Dependency
    };
    ClassConnectionItem(
            ClassDiagramItem *fromNode,
            ClassDiagramItem *toNode,
            relationsParams *params,
            ClassConnectionType type,
            uint32_t order = 0,
            QColor color = QColor(50, 45, 50, 100)
    );

    ~ClassConnectionItem();

    /** Getters, setters
     */
public:
    void setType(ClassConnectionType type) {
        _connectionType = type;
    }

    [[nodiscard]] ClassConnectionType connectionType() const {
        return _connectionType;
    }

    [[nodiscard]] ClassDiagramItem *nodeFrom() const {
        return _nodeFrom;
    }

    [[nodiscard]] ClassDiagramItem *nodeTo() const {
        return _nodeTo;
    }

    [[nodiscard]] QColor color() const {
        return _color;
    }

    [[nodiscard]] uint32_t order() const {
        return _order;
    }

    void changeOrientation() {
        _orientation = !_orientation;
        trackNodes();
    }

    void setPosLeft(QPointF pos) {
        leftNum->setPos(pos);
    }

    void setPosRight(QPointF pos) {
        rightNum->setPos(pos);
    }

    msgText *getLeftNum() const {
        return leftNum;
    }

    msgText *getRightNum() const {
        return rightNum;
    }

    msgText *getMsg() const {
        return msg;
    }

    void leftSetPos(QPointF pos) {
        leftNum->setPos(pos);
    }

    void rightSetPos(QPointF pos) {
        rightNum->setPos(pos);
    }

    void msgSetPos(QPointF pos) {
        msg->setPos(pos);
    }

    void setColor(const QColor &color) {
        _color = color;
    }

    static QFlags<Qt::TextInteractionFlag> getFlags() {
        return Qt::TextInteractionFlag::TextEditable |
               Qt::TextInteractionFlag::TextSelectableByMouse |
               Qt::TextInteractionFlag::TextSelectableByKeyboard;
    }

    bool single() {
        return _single;
    }

    /** Public functions
     */
public:
    void trackNodes();

    /** Protected methods
     */
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /** Private methods
     */
private:
    void drawLine(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    [[nodiscard]] QPolygonF lineShaper() const;
    [[nodiscard]] QPair<QPointF, QPointF> edgePoints() const;
    [[nodiscard]] QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool _single;

    /** Private attributes
     */
private:
    ClassDiagramItem *_nodeFrom;
    ClassDiagramItem *_nodeTo;
    ClassConnectionType _connectionType;
    bool _orientation = true;
    QColor _color;
    uint32_t _order;
    msgText *msg;
    msgText *leftNum;
    msgText *rightNum;
};

namespace Connections {
    enum ConnectionType {
        Synchronous,
        Asynchronous,
        Reply,
        Create,
        Delete,
        Undefined,
    };

    enum ActorType {
        Caller,
        Receiver
    };
};
using namespace Connections;

/**
 *
 */
class SequenceConnectionItem : public QGraphicsLineItem {
public:
    SequenceConnectionItem(
            SequenceDiagramItem *nodeFrom,
            SequenceDiagramItem *nodeTo,
            ConnectionType connectionType
    );

    ~SequenceConnectionItem();

public:

    void trackNodes();

    [[nodiscard]] ConnectionType connectionType() const {
        return _connectionType;
    };

    [[nodiscard]] SequenceDiagramItem *nodeFrom() const {
        return _nodeFrom;
    };

    [[nodiscard]] SequenceDiagramItem *nodeTo() const {
        return _nodeTo;
    };

    ConnectionType type() {
        return _connectionType;
    }

    void setLinEnd() {
        if (std::abs(cLine.p2().x() - cLine.p1().x()) <= 15) {
            return;
        }

//        linend = cLine.p2().x() < cLine.p1().x() ? cLine.p2() + QPointF(10, 0) :
//                 cLine.p2() - QPointF(10 ,0);
//
//        if (cLine.p2().x() < cLine.p1().x()) {
//            pUp = QPointF(linend.x() + 13, linend.y() + 8);
//            pDown = QPointF(linend.x() + 13, linend.y() - 8);
//        }
//        else {
//            pUp = QPointF(linend.x() - 13, linend.y() + 8);
//            pDown = QPointF(linend.x() - 13, linend.y() - 8);
//        }
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
    QPointF pEnd;
    QPointF pUp;
    QPointF pDown;
    QLineF cLine;
    qreal posX;
    qreal scale = 20;
    QColor _color = QColor(0,0,0,200);
    ConnectionType _connectionType;
    SequenceDiagramItem *_nodeTo;
    SequenceDiagramItem *_nodeFrom;

};

#endif //DIAGRAMS_CONNECTIONS_H
// File: SequenceConnections.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022

#define DEBUG 1

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QStyle>
#include <QPainter>

#include "Connections.h"
#include "../DiagramItem/DiagramItem.h"

using namespace Connections;

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
SequenceConnection::SequenceConnection(SequenceDiagramItem *nodeFrom,
                                       SequenceDiagramItem *nodeTo,
                                       ConnectionType connectionType,
                                       QColor clr
) {
    _nodeFrom = nodeFrom;
    _nodeTo = nodeTo;
    _connectionType = connectionType;

    _nodeFrom->addConnection(this, Caller);
    _nodeTo->addConnection(this, Receiver);

    _color = clr;

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(-1.0);

    setY(100);
    trackNodes();
}

/**
 * Changes the y-position of the line.
 * @param change
 * @param value
 * @return
 */
QVariant SequenceConnection::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        return QPointF(pos().x(), value.toPointF().y());
    return QGraphicsItem::itemChange(change, value);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    switch (_connectionType) {
        case Synchronous:
            paintSynchronous(painter, option, widget);
            break;

        case Asynchronous:
            paintAsynchronous(painter, option, widget);
            break;

        case Reply:
            paintReply(painter, option, widget);
            break;

        case Create:
            paintCreate(painter, option, widget);
            break;

        case Delete:
            paintDelete(painter, option, widget);
            break;
    }
}

/**
 *
 */
SequenceConnection::~SequenceConnection() {
    // There wiill be no destructor here, because this class is not communicating widh nodeFrom/To
//    if (_nodeFrom != nullptr) {
//        _nodeFrom->removeConnection(this);
//    }
//    if (_nodeTo != nullptr) {
//        _nodeTo->removeConnection(this);
//    }
}


/** Changing the line according to the x-position of the two nodes it is connected to.
 *
 */
void SequenceConnection::trackNodes() {
    // TODO: adjust y positions
    setLine(QLineF(QPointF(_nodeFrom->pos().x(), y()), QPointF(_nodeTo->pos().x(), y())));
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnection::paintSynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print synchronous";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnection::paintAsynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print asynchronous";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnection::paintReply(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print reply";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnection::paintCreate(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print create";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnection::paintDelete(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print delete";
}

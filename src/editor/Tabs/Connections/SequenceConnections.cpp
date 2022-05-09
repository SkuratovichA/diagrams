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

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
SequenceConnectionItem::SequenceConnectionItem(SequenceDiagramItem *fromNode,
                                               SequenceDiagramItem *toNode,
                                               ConnectionType connectionType) {
    _nodeFrom = fromNode;
    _nodeTo = toNode;
    _connectionType = connectionType;
    _nodeFrom->addConnection(this, connectionType, Caller);
    _nodeTo->addConnection(this, connectionType, Receiver);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(-1.0);
    trackNodes();
}

/**
 * Changes the y-position of the line.
 * @param change
 * @param value
 * @return
 */
QVariant SequenceConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value) {
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
void SequenceConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
//    painter->drawLine(_nodeFrom->centre().x(), y(), _nodeTo->centre().y(), y());
//    qDebug() << "paint" << _nodeFrom->width() / 2.0 << y() << _nodeTo->width() / 2.0 << y();
}

/**
 *
 */
SequenceConnectionItem::~SequenceConnectionItem() {
    if (_nodeFrom != nullptr) {
        _nodeFrom->removeConnection(this);
    }
    if (_nodeTo != nullptr) {
        _nodeTo->removeConnection(this);
    }
}

/**
 *
 * @return
 */
QColor SequenceConnectionItem::color() const {
    return pen().color();
}

/** Changing the line according to the x-position of the two nodes it is connected to.
 *
 */
void SequenceConnectionItem::trackNodes() {
//    setLine(QLineF(_nodeFrom->pos(), _nodeTo->pos()));
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintSynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print synchronous";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintAsynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print asynchronous";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintReply(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print reply";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintCreate(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print create";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintDelete(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print delete";
}

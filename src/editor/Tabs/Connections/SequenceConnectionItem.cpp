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
SequenceConnectionItem::SequenceConnectionItem(SequenceDiagramItem *nodeFrom,
                                               SequenceDiagramItem *nodeTo,
                                               ConnectionType connectionType
) {
    _nodeFrom = nodeFrom;
    _nodeTo = nodeTo;
    _connectionType = connectionType;

    //qDebug() << __FILE__;
    //qDebug() << "   creating a connection";
    _nodeFrom->addConnection(this, Caller);
    _nodeTo->addConnection(this, Receiver);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    setZValue(-1.0);
    setY(200);

    trackNodes();
    //qDebug() << "one connection";
    //qDebug() << __FILE__ << "   connection created>";
}

/**
 *
 */
SequenceConnectionItem::~SequenceConnectionItem() {
    //qDebug() << __FILE__;
    //qDebug() << "   removing connections";
    assert(_nodeFrom != nullptr && "nodeFrom must not be nullptr");
    //_nodeFrom->removeConnection(this);
    assert(_nodeTo != nullptr && "nodeTo must not be nullptr");
    //_nodeTo->removeConnection(this);
    //qDebug() << "   connectinons removed";
}

/**
 * Changes the y-position of the line.
 * @param change
 * @param value
 * @return
 */
QVariant SequenceConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange) {
        return QPointF(pos().x(), value.toPointF().y());
    }
    return QGraphicsItem::itemChange(change, value);
}

/** Changing the line according to the x-position of the two nodes it is connected to.
 *
 */
void SequenceConnectionItem::trackNodes() {
    setLine(QLineF(
            QPointF(_nodeFrom->centre().x(), y()),
            QPointF(_nodeTo->centre().x(), y())));
}

/**
 *
 * @return
 */
QRectF SequenceConnectionItem::lineShaper() const {
    // fixme: add constraints
    auto rect = QRectF(
            QPointF(_nodeFrom->centre().x(), -10),
            QPointF(_nodeTo->centre().x(), 10)
    );
    return rect;
}

QPainterPath SequenceConnectionItem::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0, 120), 0.5, Qt::DashLine));
    auto rect =lineShaper();
    auto l = (rect.bottomLeft() + rect.topLeft()) / 2;
    auto r = (rect.bottomRight() + rect.topRight()) / 2;
    painter->drawPolygon(lineShaper());

    painter->setPen(QPen(QColor(40, 10, 57, 120), 0.5, Qt::SolidLine));
    painter->drawLine(l, r);
#endif

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
 * @param painter
 * @param option
 * @param widget
 */
void
SequenceConnectionItem::paintSynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //qDebug() << "print synchronous";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void
SequenceConnectionItem::paintAsynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //qDebug() << "print asynchronous";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintReply(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //qDebug() << "print reply";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintCreate(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //qDebug() << "print create";
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintDelete(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //qDebug() << "print delete";
}


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
                                               SequenceConnectionType connectionType) {
    nodeFrom = fromNode;
    nodeTo = toNode;
    nodeFrom->addConnection(this);
    nodeTo->addConnection(this);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(-1.0);
    trackNodes();
}

QVariant SequenceConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        return QPointF(pos().x(), value.toPointF().y());
    return QGraphicsItem::itemChange(change, value);
}



void SequenceConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
//    painter->drawLine(nodeFrom->centre().x(), y(), nodeTo->centre().y(), y());
//    qDebug() << "paint" << nodeFrom->width() / 2.0 << y() << nodeTo->width() / 2.0 << y();
}

/**
 *
 */
//SequenceConnectionItem::~SequenceConnectionItem() {
//    if (nodeFrom != nullptr) {
//        nodeFrom->removeConnection(this);
//    }
//    if (nodeTo != nullptr) {
//        nodeTo->removeConnection(this);
//    }
//}

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
//    setLine(QLineF(nodeFrom->pos(), nodeTo->pos()));
}
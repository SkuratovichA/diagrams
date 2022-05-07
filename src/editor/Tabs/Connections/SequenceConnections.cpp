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
    // rememder connections
    nodeFrom = fromNode;
    nodeTo = toNode;
    nodeFrom->addConnection(this);
    nodeTo->addConnection(this);

    setPen(QPen(Qt::black));

    auto wid1 = nodeFrom->width() / 2.0;
    auto wid2 = nodeTo->width() / 2.0;
    auto line = QGraphicsLineItem(wid1, 400, wid2, 400, this);
    setLine(wid1, 200, wid2, 200);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
//    setZValue(-1.0);
    trackNodes();
}

QVariant SequenceConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        return QPointF(pos().x(), value.toPointF().y());
    return QGraphicsItem::itemChange(change, value);
}



void SequenceConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawLine(nodeFrom->centre().x(), y(), nodeTo->centre().y(), y());
    qDebug() << "paint" << nodeFrom->width() / 2.0 << y() << nodeTo->width() / 2.0 << y();
}

/**
 *
 */
SequenceConnectionItem::~SequenceConnectionItem() {
    if (nodeFrom != nullptr) {
        nodeFrom->removeConnection(this);
    }
    if (nodeTo != nullptr) {
        nodeTo->removeConnection(this);
    }
}

///**
// *
// * @param color
// */
//void SequenceConnectionItem::setColor(const QColor &color) {
//    setPen(QPen(color, 1.0));
//}

/**
 *
 * @return
 */
QColor SequenceConnectionItem::color() const {
    return pen().color();
}

/**
 *
 */
void SequenceConnectionItem::trackNodes() {
    setLine(QLineF(nodeFrom->pos(), nodeTo->pos()));
}

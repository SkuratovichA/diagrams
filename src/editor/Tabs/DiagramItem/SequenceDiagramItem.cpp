// File: SequenceDiagramItem.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022

#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QInputEvent>

/**
 * A constructor.
 *
 * This constructor creates an entity for the sequence diagram.
 * It is a simple rectangle with a child item, which represents
 * the name of the object.
 *
 * @param params class with all necessary information to create an object.
 */
SequenceDiagramItem::SequenceDiagramItem(actorParams *params, ClassDiagramItem *parentClassDiagramItem_)
        : DiagramItem(params->width(),
                      params->height(),
                      DiagramItem::Actor,
                      params->color()
) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    QFlags<Qt::TextInteractionFlag> _flags = Qt::TextInteractionFlag::TextEditable |
                                             Qt::TextInteractionFlag::TextSelectableByMouse |
                                             Qt::TextInteractionFlag::TextSelectableByKeyboard;
//    QColor colo2 = QColor(244, 120, 9, 10);
//    qDebug() << colo2.red() << colo2.green() << colo2.blue() << colo2.alpha();
    setPen(QPen(QColor(1, 0, 0, 0)));
    _head = new NameObject(this, _flags, QPointF(-3, -40), params->name());
    _parentClassDiagramItem = parentClassDiagramItem_;

    auto actorRect = new QGraphicsRectItem(0, 0, params->width(), params->height(), this);
    actorRect->setPen(QPen(QColor(Qt::black), 3.0));
    actorRect->setBrush(QBrush(color()));

//    auto lifetime = new ActorLifetime(this, QPointF(width() / 2, height()));
    setRect(boundingBox());
}


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
QVariant SequenceDiagramItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        return QPointF(value.toPointF().x(), pos().y());
    return QGraphicsItem::itemChange(change, value);
}

/**
 * Add a connection to the set of connections for certain item.
 *
 * @param connection message arrow between objects
 */
void SequenceDiagramItem::addConnection(SequenceConnectionItem *connection) {
    _connections.insert(connection);
}

/**
 * Remove a connection to the set of connections for certain item.
 *
 * @param connection message arrow between objects
 */
void SequenceDiagramItem::removeConnection(SequenceConnectionItem *connection) {
    _connections.remove(connection);
}

// File: SequenceDiagramItem.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022

#include "DiagramItem.h"
#include "SequenceDiagramLifeLine/SequenceDiagramLifeLine.h"

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
SequenceDiagramItem::SequenceDiagramItem(
        actorParams *params,
        ClassDiagramItem *parentClassDiagramItem_)
        : DiagramItem(80,
                      50,
                      DiagramItem::Actor,
                      params->color()) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    QFlags<Qt::TextInteractionFlag> _flags = Qt::TextInteractionFlag::TextEditable |
                                             Qt::TextInteractionFlag::TextSelectableByMouse |
                                             Qt::TextInteractionFlag::TextSelectableByKeyboard;

    _head = new NameObject(this, _flags, QPointF(-3, -40), params->name());
    qreal Pos = (80 - _head->boundingRect().width()) / 2;
    qDebug() << "position " << Pos;
    _head->setPos(Pos, -40);
    _parentClassDiagramItem = parentClassDiagramItem_;

    auto actorRect = new QGraphicsRectItem(0, 0, 80, 50, this);
    actorRect->setPen(QPen(QColor(Qt::black), 3.0));
    actorRect->setBrush(QBrush(params->color()));

    _lifeLine = new SequenceDiagramLifeLine(this, 0, _lineLength);

//    for (auto c: _connections) {
//        c->trackNodes();
//    }
    trackNodes();
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
QVariant SequenceDiagramItem::itemChange(
        GraphicsItemChange change,
        const QVariant &value) {
    if (change == ItemPositionChange) {
        return QPointF(std::max<qreal>(-10, value.toPointF().x()), std::max<qreal>(-10, pos().y()));
    }
    if (change == ItemPositionHasChanged) {
        for (auto c: _connections) {
            c->trackNodes();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

/**
 * Add a connection to the set of connections for certain item.
 * @param connection message arrow between objects
 */
void SequenceDiagramItem::addConnection(
        SequenceConnectionItem *connection,
        ActorType actorType) {
    qDebug() << __FILE__ << " " << __LINE__;
    qDebug() << "   adding connection";
    qDebug() << "   " << connection;
    if (_removedConnections.contains(connection)) {
        qDebug() << "    --* get this connection from /dev/null :)";
        _removedConnections.remove(connection);
    }
    if (!_connections.contains(connection)) {
        _connections.insert(connection);
    }
    _lifeLine->addConnection(connection, actorType);
    trackNodes();
}

void SequenceDiagramItem::trackNodes() {
    for (auto c: _connections) {
        c->trackNodes();
    }
    _lifeLine->trackNodes();
}

/**
 * Remove a connection to the set of connections for certain item.
 *
 * @param connection message arrow between objects
 */
void SequenceDiagramItem::removeConnection(SequenceConnectionItem *connection) {
    qDebug() << __FILE__ << " " << __LINE__;
    qDebug() << "   removing connection";
    qDebug() << "   removing << "<< connection <<"this from " << _connections;
    _removedConnections.insert(connection);
    _connections.remove(connection);
    _lifeLine->removeConnection(connection);
    trackNodes();
}

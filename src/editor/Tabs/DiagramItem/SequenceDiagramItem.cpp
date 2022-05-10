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
        : DiagramItem(params->width(),
                      params->height(),
                      DiagramItem::Actor,
                      params->color()) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    QFlags<Qt::TextInteractionFlag> _flags = Qt::TextInteractionFlag::TextEditable |
                                             Qt::TextInteractionFlag::TextSelectableByMouse |
                                             Qt::TextInteractionFlag::TextSelectableByKeyboard;
    setPen(QPen(QColor(1, 0, 0, 0)));
    _head = new NameObject(this, _flags, QPointF(-3, -40), params->name());
    qreal Pos = (params->width() - _head->boundingRect().width()) / 2;
    _head->setPos(Pos, -40);
    _parentClassDiagramItem = parentClassDiagramItem_;

    auto actorRect = new QGraphicsRectItem(0, 0, params->width(), params->height(), this);
    actorRect->setPen(QPen(QColor(Qt::black), 3.0));
    actorRect->setBrush(QBrush(color()));

    _lifeLine = new SequenceDiagramLifeLine(this, 0, _lineLength);

#if DEBUG
    lifeLine->addActiveRegion(QPair<qreal, qreal>(280, 350));
    lifeLine->addActiveRegion(QPair<qreal, qreal>(10, 20));
    lifeLine->addActiveRegion(QPair<qreal, qreal>(30, 60));
    lifeLine->addActiveRegion(QPair<qreal, qreal>(70, 80));

    lifeLine->addActiveRegion(QPair<qreal, qreal>(50, 120));
#endif

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
        qDebug() << "<tracking connection nodes";
        for (auto c : _connections) {
            c->trackNodes();
        }
        qDebug() << "connections tracked>";
    }
    return QGraphicsItem::itemChange(change, value);
}

/**
 * Add a connection to the set of connections for certain item.
 *
 * @param connection message arrow between objects
 */
void SequenceDiagramItem::addConnection(
        SequenceConnectionItem *connection,
        ActorType actorType) {
    qDebug() << "<adding connection";
    _connections.insert(connection);
    assert( _lifeLine != nullptr && "lifeLine must not be null");
    _lifeLine->addConnection(connection->y(), connection->connectionType(), actorType);
    qDebug() << "connection added>";
}

/**
 * Remove a connection to the set of connections for certain item.
 *
 * @param connection message arrow between objects
 */
void SequenceDiagramItem::removeConnection(SequenceConnectionItem *connection) {
    qDebug() << __FILE__;
    qDebug() << "   removing connection";
    qDebug() << "removing << "<< connection <<"this from " << _connections;
    _connections.remove(connection);
}

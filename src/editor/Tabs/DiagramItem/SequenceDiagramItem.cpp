/** @file SequenceDiagramItem.cpp
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @author Shchapaniak Andrei <xshcha00\@vutbr.cz>
 * @date 07.05.2022
 */


#include "DiagramItem.h"
#include "SequenceDiagramLifeLine/SequenceDiagramLifeLine.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

SequenceDiagramItem::SequenceDiagramItem(
        SequenceDiagramItemParameters *params,
        ClassDiagramItem *parentClassDiagramItem_)
        : DiagramItem(80,
                      50,
                      params->color(),
                      "Actor") {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    QFlags<Qt::TextInteractionFlag> _flags = Qt::TextInteractionFlag::TextEditable |
                                             Qt::TextInteractionFlag::TextSelectableByMouse |
                                             Qt::TextInteractionFlag::TextSelectableByKeyboard;

    _head = new NameObject(this, _flags, QPointF(-3, -40), params->name());
    qreal Pos = (80 - _head->boundingRect().width()) / 2;
    _head->setPos(Pos, -40);
    _parentClassDiagramItem = parentClassDiagramItem_;

    auto actorRect = new QGraphicsRectItem(0, 0, 80, 50, this);
    actorRect->setPen(QPen(QColor(Qt::black), 3.0));
    actorRect->setBrush(QBrush(params->color()));

    _lifeLine = new SequenceDiagramLifeLine(this, 0, _lineLength);

    trackNodes();
    setRect(boundingBox());
}

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

void SequenceDiagramItem::addConnection(
        SequenceConnectionItem *connection,
        ActorType actorType,
        CommandType::CommandType commandType,
        CommandType::RequestType requestType) {
    if (commandType == CommandType::Add) {
        if (requestType == CommandType::Connection) {
            _removedConnectionsOnAddConnection.remove(connection);
        } else { // Self
            _removedConnectionsOnAddSelf.remove(connection);
        }
    } else { // Delete
        if (requestType == CommandType::Connection) {
            _removedConnectionsOnDeleteConnection.remove(connection);
        } else { // Self
            _removedConnectionsOnDeleteSelf.remove(connection);
        }
    }
    _connections.insert(connection);
    _lifeLine->addConnection(connection, actorType);
    trackNodes();
}

void SequenceDiagramItem::trackNodes() {
    for (auto c: _connections) {
        c->trackNodes();
    }
    _lifeLine->trackNodes();
}

void SequenceDiagramItem::removeConnection(
        SequenceConnectionItem *connection,
        CommandType::CommandType commandType,
        CommandType::RequestType requestType) {
    if (commandType == CommandType::Add) {
        if (requestType == CommandType::Connection) {
            _removedConnectionsOnAddConnection.insert(connection);
        } else { // Self
            _removedConnectionsOnAddSelf.insert(connection);
        }
    } else { // Delete
        if (requestType == CommandType::Connection) {
            _removedConnectionsOnDeleteConnection.insert(connection);
        } else { // self
            _removedConnectionsOnDeleteSelf.insert(connection);
        }
    }
    _connections.remove(connection);
    _lifeLine->removeConnection(connection);
    trackNodes();
}

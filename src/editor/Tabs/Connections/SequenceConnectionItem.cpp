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
SequenceConnectionItem::SequenceConnectionItem(SequenceDiagramItem *nodeFrom,
                                               SequenceDiagramItem *nodeTo,
                                               ConnectionType connectionType
                                               ) {
    _nodeFrom = nodeFrom;
    _nodeTo = nodeTo;
    _connectionType = connectionType;

    _connections.insert(new SequenceConnection(nodeFrom, nodeTo, connectionType));
    _nodeFrom->addConnection(_connections.values().at(0), Caller);
    _nodeTo->addConnection(_connections.values().at(0), Receiver);

    if (connectionType == Synchronous) {
        _connections.insert(new SequenceConnection(nodeTo, nodeFrom, connectionType));
        _nodeFrom->addConnection(_connections.values().at(1), Caller);
        _nodeTo->addConnection(_connections.values().at(1), Receiver);
    }

    trackNodes();
}

/**
 *
 */
SequenceConnectionItem::~SequenceConnectionItem() {
    _nodeFrom->removeConnection(_connections.values().at(0));
    _nodeTo->removeConnection(_connections.values().at(0));
    if (_connectionType == Synchronous) {
        _nodeFrom->removeConnection(_connections.values().at(1));
        _nodeTo->removeConnection(_connections.values().at(1));
    }

}

/** Changing the line according to the x-position of the two nodes it is connected to.
 *
 */
void SequenceConnectionItem::trackNodes() {
    for (auto c : _connections) {
        c->trackNodes();
    }
}
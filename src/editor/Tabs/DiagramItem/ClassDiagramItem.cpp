/** @file ClassDiagramItem.cpp
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @author Shchapaniak Andrei <xshcha00\@vutbr.cz>
 * @date 03.05.2022
 */


#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QInputEvent>

ClassDiagramItem::ClassDiagramItem(ClassDiagramItemParameters *params)
        : DiagramItem(params->width(),
                      params->height(),
                      params->color(),
                      params->typeClass()) {

    QGraphicsLineItem *lineAttr;
    ClassTextAttr *textAttr;
    _flags = Qt::TextInteractionFlag::TextEditable |
             Qt::TextInteractionFlag::TextSelectableByMouse |
             Qt::TextInteractionFlag::TextSelectableByKeyboard;
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);

    int line;

    // name of the class
    setPen(QPen(color()));

    _head = new NameObject(this, _flags, QPointF(20, -40), params->name());
    qreal Pos = (params->width() - _head->boundingRect().width()) / 2;
    _head->setPos(Pos, -40);

    if (params->typeClass() == "Class") {
        line = 1;
        textAttr = new ClassTextAttr(this, "ATTRIBUTES", QPointF(tabText(), tabText()), Qt::NoTextInteraction);
        textAttr->setFont(QFont("Times", 10, QFont::Bold));

        for (auto attr_name: params->attrs()) {
            lineAttr = createLine(0, rowHeight() * line);
            _attrsLines.push_back(lineAttr);
            textAttr = new ClassTextAttr(this, attr_name, QPointF(tabText(), rowHeight() * line + tabText()), _flags);
            _attrs.push_back(textAttr);

            line++;
        }

        lineAttr = createLine(0, rowHeight() * line);
        _methodsLines.push_back(lineAttr);

        textAttr = new ClassTextAttr(this, "METHODS", QPointF(tabText(), rowHeight() * line++ + tabText()),
                                     Qt::NoTextInteraction);
        textAttr->setFont(QFont("Times", 10, QFont::Bold));
        _methods.push_back(textAttr);
    } else {
        line = 0;
        textAttr = new ClassTextAttr(this, "<<interface>>", QPointF(tabText(), rowHeight() * line++ + tabText()),
                                     Qt::NoTextInteraction);
        textAttr->setFont(QFont("Times", 12, QFont::Bold));
    }

    for (auto method_name: params->methods()) {
        lineAttr = createLine(0, rowHeight() * line);
        _methodsLines.push_back(lineAttr);
        textAttr = new ClassTextAttr(this, method_name, QPointF(tabText(), rowHeight() * line + tabText()), _flags);
        _methods.push_back(textAttr);

        line++;
    }

    setRect(boundingBox());
    setBrush(QBrush(QColor(255, 255, 255, 255)));
}

void ClassDiagramItem::addConnection(ClassConnectionItem *connection,
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
}

void ClassDiagramItem::removeConnection(ClassConnectionItem *connection,
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
}

/**
 * Handle an event of the item selection.
 * In the case of the Right button click, the item becomes a selected.
 *
 * @param event mouse event
 */
void ClassDiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        setSelected(true);
    }
}

/**
 * Notify custom items that some part of the item's state changes.
 * Reimplementation of this function provides a possibility for relation
 * arrows to track a moved item on the scene.
 *
 * @param change what was changed
 * @param value new value of changed item
 *
 * @return new value
 */
QVariant ClassDiagramItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        for (auto *connection: _connections) {
            if (connection->connectionType() == ClassConnectionItem::Generalization
                && static_cast<void *>(connection->nodeFrom()) == static_cast<void *>(this)) {
                auto commonMethods = QList<QString>();
                auto methodsFromStrings = QList<QString>();
                for (auto methodFrom: connection->nodeTo()->methods()) {
                    methodsFromStrings.append(methodFrom->toPlainText());
                }
                for (auto item: methods()) {
                    if (methodsFromStrings.contains(item->toPlainText())) {
                        commonMethods.append(item->toPlainText());
                    }
                }
                // this fix is so hot that i am horny...
                for (auto methodFrom: methods()) {
                    if (methodFrom->toPlainText() != "METHODS" && commonMethods.contains(methodFrom->toPlainText())) {
                        methodFrom->setDefaultTextColor(Qt::cyan);
                    }
                }
            } else {
                // reset color
                for (auto methodFrom: methods()) {
                    methodFrom->setDefaultTextColor(Qt::black);
                }
            }
            connection->trackNodes();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

/**
 * A destructor.
 */
ClassDiagramItem::~ClassDiagramItem() {
//            foreach (ClassConnectionItem *connection, _connections) {
//            delete connection;
//        }
}

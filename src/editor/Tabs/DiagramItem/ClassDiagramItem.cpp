// File: ClassDiagramItem.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QInputEvent>
#include "../Connections/Connections.h"

/**
 * A constructor.
 *
 * This constructor creates an entity for the class diagram.
 * TODO description
 *
 * @param params class with all necessary information to create an object.
 */
ClassDiagramItem::ClassDiagramItem(classParams *params)
        : DiagramItem(params->width(),
                      params->height(),
                      DiagramItem::Class,
                      params->color()) {

    QGraphicsLineItem *lineAttr;
    ClassTextAttr *textAttr;
    _flags = Qt::TextInteractionFlag::TextEditable |
             Qt::TextInteractionFlag::TextSelectableByMouse |
             Qt::TextInteractionFlag::TextSelectableByKeyboard;
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    int line;

    // name of the class
    setPen(QPen(color()));

    _head = new NameObject(this, _flags, QPointF(20, -40), params->name()); // i do not know why coordinates 5, -40
    qreal Pos = (params->width() - _head->boundingRect().width()) / 2;
    _head->setPos(Pos, -40);

    if (params->type() == "Class") {
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
    }
    else {
        line = 0;
        textAttr = new ClassTextAttr(this, "<<interface>>", QPointF(tabText(), rowHeight() * line++ + tabText()),Qt::NoTextInteraction);
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

/**
 * Add a connection to the set of connections for certain item.
 *
 * @param connection relation arrow between objects
 */
void ClassDiagramItem::addConnection(ClassConnectionItem *connection) {
    _connections.insert(connection);
}

/**
 * Remove a connection to the set of connections for certain item.
 *
 * @param connection relation arrow between objects
 */
void ClassDiagramItem::removeConnection(ClassConnectionItem *connection) {
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
            foreach (ClassConnectionItem *connection, _connections) {
            delete connection;
        }
}

#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QInputEvent>
// FIXME: (1) create 2 separate files from this shit. (2) create custom function for adding a line

CustomAttrText::CustomAttrText(ClassDiagramItem *p, QString text, qreal x, qreal y,
                               QFlags<Qt::TextInteractionFlag> flags)
        : QGraphicsTextItem(text, p) {
    setTextInteractionFlags(flags);
    setPos(x, y);
    _parent = p;
}

CustomAttrText::~CustomAttrText() {

}

void CustomAttrText::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        setPlainText(toPlainText().remove('\n'));
        clearFocus();
        return;
    }

    qreal maxLen = 0;

    for (auto item: parent()->attrs()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }
    for (auto item: parent()->methods()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }

    if (maxLen + 30 < parent()->width()) {
        maxLen = parent()->width() - 30;
    } else if (maxLen + 20 > parent()->width()) {
        maxLen = parent()->width() + 20;
    } else {
        return;
    }

    parent()->setRect(0, 0, maxLen, parent()->height());
    for (auto item: parent()->attrsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    for (auto item: parent()->methodsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    parent()->setWidth(maxLen);

    qreal midW = parent()->_head->boundingRect().width();
    qreal midO = parent()->width();
    parent()->_head->setPos((midO - midW) / 2, -40);
    for (auto x : this->parent()->connections()) {
        x->trackNodes();
    }
}

ActorDiagramItem::ActorDiagramItem(actorParams *params)
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
    _head = new NameObject(this, _flags, -10, -40, params->name());

    auto setpen = [this](QGraphicsLineItem *l) {l->setPen(QPen(color(), 3.0));};
    auto legL = new QGraphicsLineItem(35.0, 70.0, 10.0, 110.0, this);
    setpen(legL);
    auto legR = new QGraphicsLineItem(35.0, 70.0, 60.0, 110.0, this);
    setpen(legR);
    auto handL = new QGraphicsLineItem(35.0, 45.0, 0.0, 35.0, this);
    setpen(handL);
    auto handR = new QGraphicsLineItem(35.0, 45.0, 70.0, 35.0, this);
    setpen(handR);
    auto body = new QGraphicsLineItem(35.0, 30.0, 35.0, 70.0, this);
    setpen(body);
    auto head = new QGraphicsEllipseItem(20.0, 0.0, 30.0, 30.0, this);
    head->setPen(QPen(color(), 3.0));
    head->setBrush(QBrush(color()));
    setRect(boundingBox());
}

/**
 *
 * @param connection
 */
void ActorDiagramItem::addConnection(ActorConnectionItem *connection) {
    _connections.insert(connection);
}

/**
 *
 * @param connection
 */
void ActorDiagramItem::removeConnection(ActorConnectionItem *connection) {
    _connections.remove(connection);
}

/**
 *
 * @param item
 */
ClassDiagramItem::ClassDiagramItem(classParams *params)
        : DiagramItem(params->width(),
                      params->height(),
                      DiagramItem::Class,
                      params->color()) {

    _rowHeight = (params->height() / 4.0);
    _rowWidth = params->width();
    _tabText = _rowHeight / 15.0;
    QGraphicsLineItem *lineAttr;
    CustomAttrText *textAttr;
    _flags = Qt::TextInteractionFlag::TextEditable |
             Qt::TextInteractionFlag::TextSelectableByMouse |
             Qt::TextInteractionFlag::TextSelectableByKeyboard;
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    int line = 1;

    // name of the class
    setPen(QPen(color()));

    _head = new NameObject(this, _flags, 5, -40, params->name()); // i do not why coordinates 5, -40

    textAttr = new CustomAttrText(this, "ATTRIBUTES", _tabText, _tabText, Qt::NoTextInteraction);
    textAttr->setFont(QFont("Times", 10, QFont::Bold));

    for (auto attr_name: params->attrs()) {
        lineAttr = createLine(0, _rowHeight * line);
        _attrsLines.push_back(lineAttr);

        textAttr = new CustomAttrText(this, attr_name, _tabText, _rowHeight * line + _tabText, _flags);
        _attrs.push_back(textAttr);

        line++;
    }

    lineAttr = createLine(0, _rowHeight * line);
    _methodsLines.push_back(lineAttr);

    textAttr = new CustomAttrText(this, "METHODS", _tabText, _rowHeight * line++ + _tabText, Qt::NoTextInteraction);
    textAttr->setFont(QFont("Times", 10, QFont::Bold));
    _methods.push_back(textAttr);

    for (auto method_name: params->methods()) {
        lineAttr = createLine(0, _rowHeight * line);
        _methodsLines.push_back(lineAttr);

        textAttr = new CustomAttrText(this, method_name, _tabText, _rowHeight * line + _tabText, _flags);
        _methods.push_back(textAttr);

        line++;
    }

    setRect(boundingBox());
    setBrush(QBrush(QColor(255, 255, 255, 255)));
}

/**
 *
 */
ClassDiagramItem::~ClassDiagramItem() {
    foreach (ClassConnectionItem *connection, _connections) {
        delete connection;
        qDebug() << "Connection deleted (diagramItem.cpp)";
    }
}

/**
 *
 * @param connection
 */
void ClassDiagramItem::addConnection(ClassConnectionItem *connection) {
    _connections.insert(connection);
}

/**
 *
 * @param connection
 */
void ClassDiagramItem::removeConnection(ClassConnectionItem *connection) {
    _connections.remove(connection);
}

/**
 *
 * @param change
 * @param value
 * @return
 */
QVariant ClassDiagramItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        for (auto *connection: _connections) {
            connection->trackNodes();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

/**
 *
 * @param parent
 * @param flags
 * @param x
 * @param y
 */
NameObject::NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y, QString str)
        : QGraphicsTextItem(str, parent) {
    _parent = parent;
    setPos(x, y);
    setFont(QFont("Courier", 20));
    setTextInteractionFlags(flags);
    topLevelItem();
}

/**
 *
 * @param event
 */
void NameObject::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        setPlainText(toPlainText().remove('\n'));
        clearFocus();
        return;
    }
    ClassDiagramItem *tmp1 = dynamic_cast<ClassDiagramItem *>(parent());
    ActorDiagramItem *tmp2 = dynamic_cast<ActorDiagramItem *>(parent());
    qreal midO = tmp1 == nullptr ? tmp2->width() : tmp1->width();
    qreal midW = boundingRect().width();
    setPos((midO - midW) / 2, -40);
}

void ClassDiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        setSelected(true);
    }
}

NameObject::~NameObject() {

}

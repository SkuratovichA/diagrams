#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
// FIXME: (1) create 2 separate files from this shit. (2) create custom function for adding a line


ActorDiagramItem::ActorDiagramItem(QGraphicsItem *item)
        : QGraphicsRectItem(item), DiagramItem(70, 110, DiagramItem::Actor) {

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    setPen(QPen(QColor(1, 0, 0, 0)));
    auto text = new QGraphicsTextItem("test", this);
    text->setTextInteractionFlags(
            Qt::TextInteractionFlag::TextEditable | Qt::TextInteractionFlag::TextSelectableByMouse |
            Qt::TextInteractionFlag::TextSelectableByKeyboard);
    text->topLevelItem();

    auto setpen = [this](QGraphicsLineItem *l) { l->setPen(QPen(color(), 3)); };
    auto legL = new QGraphicsLineItem(35, 70, 10, 110, this);
    setpen(legL);
    auto legR = new QGraphicsLineItem(35, 70, 60, 110, this);
    setpen(legR);
    auto handL = new QGraphicsLineItem(35, 45, 0, 35, this);
    setpen(handL);
    auto handR = new QGraphicsLineItem(35, 45, 70, 35, this);
    setpen(handR);
    auto body = new QGraphicsLineItem(35, 30, 35, 70, this);
    setpen(body);
    auto head = new QGraphicsEllipseItem(20, 0, 30, 30, this);
    head->setPen(QPen(color(), 3));
    head->setBrush(QBrush(color()));
    setRect(boundingBox());
}

/**
 *
 * @param connection
 */
void ActorDiagramItem::addConnection(ActorConnectionItem *connection) {
    connections.insert(connection);
}

void ActorDiagramItem::removeConnection(ActorConnectionItem *connection) {
    connections.remove(connection);
}

/**
 *
 * @param item
 */
ClassDiagramItem::ClassDiagramItem(QGraphicsItem *item)
        : QGraphicsRectItem(item), DiagramItem(120, 120, DiagramItem::Class)  {

    _rowHeight = 30;
    _rowWidth = 120;
    _tabText = 2;
    QGraphicsLineItem *lineAttr;
    CustomAttrText *textAttr;
    _flags = Qt::TextInteractionFlag::TextEditable |
             Qt::TextInteractionFlag::TextSelectableByMouse |
             Qt::TextInteractionFlag::TextSelectableByKeyboard;


    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    // name of the class
    setPen(QPen(color()));

    _head = new NameObject(this, _flags);

    textAttr = new CustomAttrText(this, "ATTRIBUTES", _tabText, _tabText,  Qt::NoTextInteraction);

    lineAttr = createLine(0, _rowHeight);
    attrsLines.push_back(lineAttr);

    textAttr = new CustomAttrText(this, "+ int name", _tabText, _rowHeight + _tabText, _flags);
    attrs.push_back(textAttr);

    lineAttr = createLine(0, _rowHeight * 2);
    methodsLines.push_back(lineAttr);

    textAttr = new CustomAttrText(this, "METHODS", _tabText, _rowHeight * 2 + _tabText, Qt::NoTextInteraction);
    methods.push_back(textAttr);

    lineAttr = createLine(0, _rowHeight * 3);
    methodsLines.push_back(lineAttr);

    textAttr = new CustomAttrText(this, "+ int name()", _tabText, _rowHeight * 3 + _tabText, _flags);
    methods.push_back(textAttr);

    setRect(boundingBox());
    setBrush(QBrush(QColor(255,255,255,255)));
}

CustomAttrText::CustomAttrText(ClassDiagramItem *p, QString text, qreal x, qreal y, QFlags<Qt::TextInteractionFlag> flags)
                            : QGraphicsTextItem(text, p) {
    setTextInteractionFlags(flags);
    setPos(x, y);
    _parent = p;
}

void CustomAttrText::keyReleaseEvent(QKeyEvent *event) {
    qreal maxLen = 0;

    for (auto item : parent()->getAttrs()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }
    for (auto item : parent()->getMethods()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }

    if (maxLen + 30 < parent()->width()) {
        maxLen = parent()->width() - 30;
    }
    else if (maxLen + 20 > parent()->width()) {
        maxLen = parent()->width() + 20;
    }
    else {
        return;
    }

    parent()->setRect(0, 0, maxLen, parent()->height());
    for (auto item : parent()->getAttrsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    for (auto item : parent()->getMethodsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    parent()->setWidth(maxLen);

    qreal midW = parent()->_head->boundingRect().width();
    qreal midO = parent()->width();
    parent()->_head->setPos((midO - midW) / 2, -40);
}

NameObject::NameObject(ClassDiagramItem *parent, QFlags<Qt::TextInteractionFlag> flags)
            : QGraphicsTextItem("_Name_", parent) {
    _parent = parent;
    setPos(5, -40);
    setFont(QFont("Courier", 20));
    setTextInteractionFlags(flags);
    topLevelItem();
}

void NameObject::keyReleaseEvent(QKeyEvent *event) {
    qreal midW = boundingRect().width();
    qreal midO = parent()->width();
    setPos((midO - midW) / 2, -40);
}

NameObject::~NameObject() {

}

/**
 *
 * @param connection
 */
void ClassDiagramItem::addConnection(ClassConnectionItem *connection) {
    connections.insert(connection);
}

/**
 *
 * @param connection
 */
void ClassDiagramItem::removeConnection(ClassConnectionItem *connection) {
    connections.remove(connection);
}

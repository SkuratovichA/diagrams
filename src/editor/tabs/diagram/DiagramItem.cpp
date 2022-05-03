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
    QGraphicsTextItem *textAttr;

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    // name of the class
    setPen(QPen(color()));
    auto text = new QGraphicsTextItem("name", this);
    text->setPos(-30, -30);
    text->setTextWidth(width()+80);
    text->setFont(QFont("Courier", 20));
    setTextFlags(text);
    text->topLevelItem();

    auto attrs_st = new QGraphicsTextItem("ATTRIBUTES", this);
    attrs_st->setPos(_tabText, _tabText);

    lineAttr = createLine(0, _rowHeight);
    attrsLines.push_back(lineAttr);

    textAttr = createText(_tabText, _rowHeight + _tabText, "+ int name");
    attrs.push_back(textAttr);

    lineAttr = createLine(0, _rowHeight * 2);
    methodsLines.push_back(lineAttr);

    auto methods_st = new QGraphicsTextItem("METHODS", this);
    methods_st->setPos(_tabText, _rowHeight * 2 + _tabText);
    methods.push_back(methods_st);

    lineAttr = createLine(0, _rowHeight * 3);
    methodsLines.push_back(lineAttr);

    textAttr = createText(_tabText, _rowHeight * 3 + _tabText, "+ int name()");
    methods.push_back(textAttr);

    setRect(boundingBox());
    setBrush(QBrush(QColor(255,255,255,255)));

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

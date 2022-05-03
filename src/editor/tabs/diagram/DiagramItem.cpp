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
        : QGraphicsRectItem(item), DiagramItem(110, 120, DiagramItem::Class)  {

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    setPen(QPen(color()));
    auto text = new QGraphicsTextItem("name", this);
    text->setPos(-30, -30);
    text->setTextWidth(width()+80);
    text->setFont(QFont("Courier", 20));
    text->setTextInteractionFlags(
            Qt::TextInteractionFlag::TextEditable | Qt::TextInteractionFlag::TextSelectableByMouse |
            Qt::TextInteractionFlag::TextSelectableByKeyboard);
    text->topLevelItem();


    auto set_text_flags = [](QGraphicsTextItem *l) { l->setTextInteractionFlags(
            Qt::TextInteractionFlag::TextEditable | Qt::TextInteractionFlag::TextSelectableByMouse |
            Qt::TextInteractionFlag::TextSelectableByKeyboard); };

    auto attrs_st = new QGraphicsTextItem("ATTRIBUTES", this);
    set_text_flags(attrs_st);
    attrs_st->setPos(2, 2);

    auto line1 = new QGraphicsLineItem(0,30, width(), 30, this);

    auto tmp_attr = new QGraphicsTextItem("+ int name", this);
    set_text_flags(tmp_attr);
    tmp_attr->setPos(2, 32);
    attrs.push_back(tmp_attr);

    auto line2 = new QGraphicsLineItem(0, 60, width(), 60, this);

    auto methods_st = new QGraphicsTextItem("METHODS", this);
    set_text_flags(methods_st);
    methods_st->setPos(2, 62);

    auto line3 = new QGraphicsLineItem(0, 90, width(), 90, this);

    auto tmp_method = new QGraphicsTextItem("+ int name()", this);
    set_text_flags(tmp_method);
    tmp_method->setPos(2, 92);
    attrs.push_back(tmp_method);

    setRect(boundingBox());
    setBrush(QBrush(QColor(255,255,255,255)));
    refactor_table();
}

void ClassDiagramItem::refactorTable()
{
    qreal x = this->x();
    qreal y = this->y();
    setWidth(this->rect().width());
    setHeight(this->rect().height());

    this->setPos(100, 100);

    auto set_text_flags = [](QGraphicsTextItem *l) { l->setTextInteractionFlags(
            Qt::TextInteractionFlag::TextEditable | Qt::TextInteractionFlag::TextSelectableByMouse |
            Qt::TextInteractionFlag::TextSelectableByKeyboard); };

    QList<QGraphicsTextItem *> tmp_attrs;
    QList<QGraphicsTextItem *> tmp_methods;

    foreach(QGraphicsItem *obj, this->childItems()) {
        qDebug() << obj;
    }

    foreach (QGraphicsTextItem *val, this->attrs) {
        tmp_attrs.push_back(val);
        qDebug() << val->toPlainText();
    }

    foreach (QGraphicsTextItem *val, this->methods) {
        tmp_methods.push_back(val);
        qDebug() << val->toPlainText();
    }

    //QList<QGraphicsTextItem *> attrs = { QGraphicsTextItem("ATTRIBUTES", this) };



    //setX(200);
    //setY(200);
    //this->update(100, 100, 500, 500);

    //delete this;
//
//    boxRect = QRectF(0, 0, width, height+30);
////    this = new QGraphicsRectItem();
//            setRect(boxRect);

//    auto attrs_st = new QGraphicsTextItem("ATTRIBUTES", this);
//    set_text_flags(attrs_st);
//    attrs_st->setPos(2, 2);
//
//    auto line1 = new QGraphicsLineItem(0,30, width, 30, this);
//
//    auto tmp_attr = new QGraphicsTextItem("+ int name", this);
//    set_text_flags(tmp_attr);
//    tmp_attr->setPos(2, 32);
//    attrs.push_back(tmp_attr);
//
//    auto line2 = new QGraphicsLineItem(0, 60, width, 60, this);
//
//    auto methods_st = new QGraphicsTextItem("METHODS", this);
//    set_text_flags(methods_st);
//    methods_st->setPos(2, 62);
//
//    auto line3 = new QGraphicsLineItem(0, 90, width, 90, this);
//
//    auto tmp_method = new QGraphicsTextItem("+ int name()", this);
//    set_text_flags(tmp_method);
//    tmp_method->setPos(2, 92);
//    attrs.push_back(tmp_method);
}
//
//QPointF ClassDiagramItem::pos() {
//    return { x() + width / 2, y() + height / 2};
//}

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

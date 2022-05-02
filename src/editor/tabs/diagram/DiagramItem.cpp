#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>

ActorDiagramItem::ActorDiagramItem(QGraphicsItem *item)
        : QGraphicsRectItem(item) {
    QColor color(QRandomGenerator::global()->bounded(256),
                 QRandomGenerator::global()->bounded(256),
                 QRandomGenerator::global()->bounded(256));
    setPen(QPen(QColor(0, 0, 0, 0)));
    auto text = new QGraphicsTextItem("test", this);
    text->setTextInteractionFlags(
            Qt::TextInteractionFlag::TextEditable | Qt::TextInteractionFlag::TextSelectableByMouse |
            Qt::TextInteractionFlag::TextSelectableByKeyboard);
    text->topLevelItem();

    boxRect = QRectF(0, 0, 70, 110);
    auto setpen = [&color](QGraphicsLineItem *l) { l->setPen(QPen(color, 3)); };
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
    head->setPen(QPen(color, 3));
    head->setBrush(QBrush(color));
    setRect(boxRect);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

ClassDiagramItem::ClassDiagramItem(QGraphicsItem *item)
        : QGraphicsRectItem(item) {
    QColor color(QRandomGenerator::global()->bounded(256),
                 QRandomGenerator::global()->bounded(256),
                 QRandomGenerator::global()->bounded(256),
                 180);
    auto width = 140;
    setPen(QPen(color));
    auto text = new QGraphicsTextItem("name will be here", this);
    text->setPos(-30, -30);
    text->setFont(QFont("Courier", 20));
    text->setTextInteractionFlags(
            Qt::TextInteractionFlag::TextEditable | Qt::TextInteractionFlag::TextSelectableByMouse |
            Qt::TextInteractionFlag::TextSelectableByKeyboard);
    text->topLevelItem();

    boxRect = QRectF(0, 0, width, 100);
    setRect(boxRect);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}
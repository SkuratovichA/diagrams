#include "object.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>

Class::Class(qreal x, qreal y, qreal s) : QGraphicsRectItem(0, 0, 70*s, 110*s) {

}

Relation::Relation(qreal x, qreal y, qreal s) : QGraphicsRectItem(0, 0, 70*s, 110*s) {

}

Message::Message(qreal x, qreal y, qreal s) : QGraphicsRectItem(0, 0, 70*s, 110*s) {

}

Actor::Actor(qreal x, qreal y, qreal s) : QGraphicsRectItem(0, 0, 70*s, 110*s) {

      this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
      this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

      legL = new QGraphicsLineItem(35*s, 70*s, 10*s, 110*s, this);
      setPen(QPen(Qt::white));

      legR = new QGraphicsLineItem(35*s, 70*s, 60*s, 110*s, this);
      handL = new QGraphicsLineItem(35*s, 45*s, 0*s, 35*s, this);
      handR = new QGraphicsLineItem(35*s, 45*s, 70*s, 35*s, this);
      body = new QGraphicsLineItem(35*s, 30*s, 35*s, 70*s, this);
      head = new QGraphicsEllipseItem(20*s, 0*s, 30*s, 30*s, this);

      name = new QGraphicsTextItem("Name", this);
      mid =70*s/2.0;
      setText(name, "Name", mid);

      this->setPos(x, y);
}

void Actor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, cur_str);
    if (!text.isEmpty())
        setText(this->name, text, this->mid);
}

void Actor::setText(QGraphicsTextItem *item, QString text, qreal mid)
{
    cur_str = text;
    item->setPlainText(text);
    QFontMetricsF metrics{qApp->font()};
    QRectF rect = metrics.boundingRect(text);
    qreal pos_x = mid - rect.width()/2.0;
    item->setPos(pos_x, 120);
}

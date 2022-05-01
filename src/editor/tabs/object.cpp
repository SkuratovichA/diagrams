#include "object.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>

Object::Object(qreal x, qreal y, qreal s, QGraphicsItem *parent) : QGraphicsRectItem(x,y,100,100) {

      //point = new QGraphicsEllipseItem(20, 20, 10, 10);
      this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
      this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

      //drawRect(x,y,width,height);
      //new QGraphicsRectItem(x,y,width,height);
      legL = new QGraphicsLineItem(20, 100, 100, 20, this);
      legR = new QGraphicsLineItem(60, 60, 120, 120, this);
      name = new QGraphicsTextItem("allo blyat", this);
      name->setPos(90, 120);
      //line1->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
      //line1->setFlag(QGraphicsItem::ItemSendsGeometryChanges);


//    myTextColor = Qt::darkGreen;
//    myOutlineColor = Qt::darkBlue;
//    myBackgroundColor = Qt::white;

//    setFlags(ItemIsMovable | ItemIsSelectable);
//    setFlag(ItemSendsGeometryChanges);

//    r_scale = 1.0;
//    x_pos = 0;
//    y_pos = 0;
}

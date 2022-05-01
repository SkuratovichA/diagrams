#ifndef Object_H
#define Object_H

#include <QString>
#include <QColor>
#include <QGraphicsItem>
#include <QApplication>
#include <QSet>
#include <QKeyEvent>

class Object : public QGraphicsRectItem
{
    Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Object(qreal x, qreal y, qreal s, QGraphicsItem *parent = nullptr);

private:
    QGraphicsLineItem *legL;
    QGraphicsLineItem *legR;
    QGraphicsLineItem *handL;
    QGraphicsLineItem *handR;
    QGraphicsLineItem *body;
    QGraphicsEllipseItem *head;
    QGraphicsTextItem *name;
};

#endif // Object_H

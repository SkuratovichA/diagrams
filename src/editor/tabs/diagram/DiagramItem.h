#ifndef Object_H
#define Object_H

#include <QString>
#include <QColor>
#include <QGraphicsItem>
#include <QApplication>
#include <QSet>
#include <QKeyEvent>

class DiagramItem : public QGraphicsRectItem
{
    Q_DECLARE_TR_FUNCTIONS(Object);

public:
    enum { Type = UserType + 1 };
    enum EntityType { Actor = 0, Class, ActorConnection, ClassConnection, UNKNOWN_TYPE };

    DiagramItem(qreal x, qreal y, qreal s = 1.0);

public:
    // 1. FIXME: make the whole class abstract
    // 2. Make the function virtual and each class to return its type
    /*virtual*/ EntityType getEntityType();
    void setText(QGraphicsTextItem *item, QString text, qreal mid);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QGraphicsLineItem *legL;
    QGraphicsLineItem *legR;
    QGraphicsLineItem *handL;
    QGraphicsLineItem *handR;
    QGraphicsLineItem *body;
    QGraphicsEllipseItem *head;
    QGraphicsTextItem *name;

    QString cur_str = "Name";
    qreal mid;
};

class Class : public QGraphicsRectItem
{
    Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Class(qreal x, qreal y, qreal s = 1.0);

private:
    QGraphicsTextItem *name;
    QList<QGraphicsTextItem *> *attrs; // i dont sure
    QList<QGraphicsTextItem *> *methods; // i dont sure

    QString cur_str = "Name";
};

class Relation : public QGraphicsRectItem
{
    Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Relation(qreal x, qreal y, qreal s = 1.0);

private:
    QGraphicsTextItem *name;
    QList<QGraphicsTextItem *> *attrs; // i dont sure
    QList<QGraphicsTextItem *> *methods; // i dont sure

    // 2 parents

    QString cur_str = "Name";
};

class Message : public QGraphicsRectItem
{
    Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Message(qreal x, qreal y, qreal s = 1.0);

private:
    QGraphicsTextItem *name;
    QList<QGraphicsTextItem *> *attrs; // i dont sure
    QList<QGraphicsTextItem *> *methods; // i dont sure

    // 2 parents

    QString cur_str = "Name";
};

#endif // Object_H

#ifndef Object_H
#define Object_H

#include <QString>
#include <QColor>
#include <QGraphicsItem>
#include <QApplication>
#include <QSet>
#include <QKeyEvent>

class Actor : public QGraphicsRectItem
{
    Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Actor(qreal x, qreal y, qreal s = 1.0);
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

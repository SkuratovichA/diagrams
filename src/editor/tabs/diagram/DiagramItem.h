#ifndef Object_H
#define Object_H

#include <QString>
#include <QColor>
#include <QGraphicsItem>
#include <QApplication>
#include <QSet>
#include <QKeyEvent>

#include <QGraphicsPolygonItem>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPointF;
QT_END_NAMESPACE

class DiagramItem : public QGraphicsPolygonItem {
public:
    enum {
        Type = UserType + 1
    };
    enum DiagramType {
        Actor = 0, Class, ActorConnection, ClassConnection, UNKNOWN_TYPE
    };

    explicit DiagramItem(DiagramType diagramType, QGraphicsItem *item = nullptr);

    DiagramType getDiagramType() {
        return polygon() == boxPolygon ? Class : Actor;
    }

    int type() const override { return Type; }

private:
    QPolygonF boxPolygon;
    QPolygonF trianglePolygon;
};


//
//class DiagramItem : public QGraphicsRectItem
//{
//    Q_DECLARE_TR_FUNCTIONS(Object);
//
//public:
//    enum { Type = UserType + 1 };
//    enum EntityType { Actor = 0, Class, ActorConnection, ClassConnection, UNKNOWN_TYPE };
//
//    DiagramItem();
//
//public:
//    virtual EntityType getDiagramType();
//
//protected:
//    EntityType type;
//    // FIXME i am not sure if it is necessary
//    QGraphicsRectItem *item{};
//};

class Actor : public QGraphicsRectItem {
Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Actor(qreal x, qreal y, qreal s = 1.0);

public:
    //virtual EntityType getDiagramType();
    void setText(QGraphicsTextItem *item, const QString &text, qreal mid);

protected:
    QGraphicsRectItem *item{};

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

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

class Class : public QGraphicsRectItem {
Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Class(qreal x, qreal y, qreal s = 1.0);

    //EntityType getDiagramType() override;

protected:
    QGraphicsRectItem *item{};

private:
    QGraphicsTextItem *name;
    QList<QGraphicsTextItem *> *attrs; // I'm not sure
    QList<QGraphicsTextItem *> *methods; // I'm not sure

    QString cur_str = "Name";
};

#endif // Object_H

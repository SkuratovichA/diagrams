#ifndef Object_H
#define Object_H

#include <QString>
#include <QColor>
#include <QGraphicsItem>
#include <QApplication>
#include <QSet>
#include <QKeyEvent>

#include <QGraphicsPolygonItem>
#include "Connections.h"

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPointF;
QT_END_NAMESPACE

class ActorDiagramItem : public QGraphicsRectItem {
//    Q_DECLARE_TR_FUNCTIONS(ActorDiagramItem)

public:
    explicit ActorDiagramItem(QGraphicsItem *item = nullptr);

    void addConnection(ActorConnectionItem *connection);
    void removeConnection(ActorConnectionItem *connection);
//    QPointF pos();

private:
    qreal height = 110;
    qreal width = 70;
    QSet<ActorConnectionItem *> connections;
    QRectF boxRect;
};

class ClassDiagramItem : public QGraphicsRectItem {
//    Q_DECLARE_TR_FUNCTIONS(ClassDiagramItem)

public:
    explicit ClassDiagramItem(QGraphicsItem *item = nullptr);
    void refactor_table();

    void addConnection(ClassConnectionItem *connection);
    void removeConnection(ClassConnectionItem *connection);
//    QPointF pos();

private:
    qreal height = 120;
    qreal width = 140;
    QSet<ClassConnectionItem *> connections;
    QRectF boxRect;
    QList<QGraphicsTextItem *> attrs;
    QList<QGraphicsTextItem *> methods;
    QColor custom_color;
};

#endif // Object_H

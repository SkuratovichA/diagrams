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

class ActorDiagramItem : public QGraphicsRectItem {
    explicit ActorDiagramItem(QGraphicsItem *item = nullptr);

private:
    QRectF boxRect;
};

class ClassDiagramItem : public QGraphicsRectItem {
    explicit ClassDiagramItem(QGraphicsItem *item = nullptr);

private:
    QRectF boxRect;
};
#endif // Object_H

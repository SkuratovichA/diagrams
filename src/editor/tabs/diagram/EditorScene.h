// File: EditorScene.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022


#ifndef DIAGRAMS_EDITORSCENE_H
#define DIAGRAMS_EDITORSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "DiagramItem.h"

class ActorDiagramItem;
QT_BEGIN_NAMESPACE
class QGraphicsSceneDragDropEvent;
class QGraphicsViewItem;
QT_END_NAMESPACE

class EditorScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit EditorScene(QObject *parent = nullptr);

signals:
    void itemMoved(ActorDiagramItem *movedItem, const QPointF &movedFromPosition);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


private:
    QGraphicsItem *movingItem = nullptr;
    QPointF oldPos;

};


#endif //DIAGRAMS_EDITORSCENE_H

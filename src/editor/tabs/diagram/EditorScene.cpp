// File: EditorScene.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022

#include <QGraphicsSceneMouseEvent>

#include "EditorScene.h"

EditorScene::EditorScene(QObject *parent)
        : QGraphicsScene(parent)
{}

/**
 *
 * @param event
 */
void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
                     event->buttonDownScenePos(Qt::LeftButton).y());
    const QList<QGraphicsItem *> itemList = items(mousePos);
    movingItem = itemList.isEmpty() ? nullptr : itemList.first();

    if (movingItem != nullptr && event->button() == Qt::LeftButton) {
        oldPos = movingItem->pos();
    }

    //TODO:
    //clearSelection();
    QGraphicsScene::mousePressEvent(event);
}

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (movingItem != nullptr && event->button() == Qt::LeftButton) {
        if (oldPos != movingItem->pos()) {
            emit itemMoved(qgraphicsitem_cast<ActorDiagramItem *>(movingItem), oldPos);
        }
        movingItem = nullptr;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

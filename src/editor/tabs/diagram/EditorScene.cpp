// File: EditorScene.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 01.05.2022

#include <QGraphicsSceneMouseEvent>

#include "EditorScene.h"

EditorScene::EditorScene(QObject *parent)
        : QGraphicsScene(parent)
{
    multSelect = false;
}

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

    QList<QGraphicsItem *> selItems = selectedItems();
    QList<QGraphicsItem *> allItems = items();

    changeZval(selItems, 1);
    for (auto x : selItems) {
        allItems.removeOne(x);
    }

    if (multSelect == false) {
        clearSelection();
    }

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

    QList<QGraphicsItem *> selItems = selectedItems();
    QList<QGraphicsItem *> allItems = items();

    changeZval(selItems, 1);
    for (auto x : selItems) {
        allItems.removeOne(x);
    }
    changeZval(allItems, -1);

    QGraphicsScene::mouseReleaseEvent(event);
}

void EditorScene::changeZval(QList<QGraphicsItem *> items, int val) {
    ClassDiagramItem *ptr1;
    ActorDiagramItem *ptr2;

    for (auto x : items) {
        //qDebug() << x->type();
        ptr1 = dynamic_cast<ClassDiagramItem *>(x);
        ptr2 = dynamic_cast<ActorDiagramItem *>(x);

        if (ptr1 == nullptr && ptr2 == nullptr) {
            continue;
        }

        x->setZValue(val);
    }
    update();
}

void EditorScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control) {
        multSelect = true;
    }
    QGraphicsScene::keyPressEvent(event);
}

void EditorScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control) {
        multSelect = false;
    }
    QGraphicsScene::keyReleaseEvent(event);
}

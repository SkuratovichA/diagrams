// File: EditorScene.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include <QGraphicsSceneMouseEvent>

#include "EditorScene.h"

EditorScene::EditorScene(QObject *parent)
        : QGraphicsScene(parent) {
    multSelect = false;
}

/**
 *
 * @param event
 */
void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
                     event->buttonDownScenePos(Qt::LeftButton).y());
    QList<QGraphicsItem *> itemList = items(mousePos);

    if (itemList.isEmpty()) {
        movingItem = nullptr;
    }
    else {
        movingItem = itemList.first()->parentItem() == nullptr ?
                itemList.first() :
                itemList.first()->parentItem() ;
    }

    if (movingItem != nullptr && event->button() == Qt::LeftButton) {
        oldPos = movingItem->pos();
    }

    changeZval();

    if (multSelect == false) {
        clearSelection();
    }

    QGraphicsScene::mousePressEvent(event);
}

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (movingItem != nullptr && event->button() == Qt::LeftButton) {
        if (oldPos != movingItem->pos()) {
            emit itemMoved(qgraphicsitem_cast<SequenceDiagramItem *>(movingItem), oldPos);
        }
        movingItem = nullptr;
    }

    QList<QGraphicsItem *> selItems = selectedItems();

    for (auto x: selItems) {
        QList<QGraphicsItem *> collid = collidingItems(x);
        x->setZValue(1);

        for (auto y: collid) {
            y->setZValue(-1);
        }
    }

    changeZval();
    QGraphicsScene::mouseReleaseEvent(event);
}

void EditorScene::changeZval() {
    QList<QGraphicsItem *> selItems = selectedItems();
    ClassDiagramItem *ptr1;
    SequenceDiagramItem *ptr2;

    for (auto x: selItems) {
        ptr1 = dynamic_cast<ClassDiagramItem *>(x);
        ptr2 = dynamic_cast<SequenceDiagramItem *>(x);

        if (ptr1 == nullptr && ptr2 == nullptr) {
            continue;
        }

        QList<QGraphicsItem *> collid = collidingItems(x);
        x->setZValue(1);

        for (auto y: collid) {
            y->setZValue(-1);
        }
    }
    update();
}

void EditorScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        multSelect = true;
    }
    QGraphicsScene::keyPressEvent(event);
}

void EditorScene::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        multSelect = false;
    }
    QGraphicsScene::keyReleaseEvent(event);
}

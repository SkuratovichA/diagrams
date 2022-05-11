// File: EditorScene.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#ifndef DIAGRAMS_EDITORSCENE_H
#define DIAGRAMS_EDITORSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "../DiagramItem/DiagramItem.h"

class SequenceDiagramItem;

QT_BEGIN_NAMESPACE
class QGraphicsSceneDragDropEvent;

class QGraphicsViewItem;

QT_END_NAMESPACE

class EditorScene : public QGraphicsScene {
Q_OBJECT

public:
    explicit EditorScene(QObject *parent = nullptr);
    void changeZval();

signals:
    void itemMoved(QGraphicsItem *movedItem, const QPointF &movedFromPosition);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    bool multSelect;
    QGraphicsItem *movingItem = nullptr;
    QPointF oldPos;

};

#endif //DIAGRAMS_EDITORSCENE_H

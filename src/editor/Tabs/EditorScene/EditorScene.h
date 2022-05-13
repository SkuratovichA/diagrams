/** @file EditorScene.h
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @author Shchapaniak Andrei <xshcha00\@vutbr.cz>
 * @date 07.05.2022
 */


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

/**
 * @brief An editor scene class containing all logic of the program
 */
class EditorScene : public QGraphicsScene {
Q_OBJECT

public:
    /**
     * @brief A constructor.
     * @param parent paints object
     */
    explicit EditorScene(QObject *parent = nullptr);

    /**
     * @brief Change z value of items without arrows
     */
    void changeZval();

signals:
    void itemMoved(QGraphicsItem *movedItem, const QPointF &movedFromPosition);

protected:

    /**
     * @brief Handle a mouse clicking and change a Z value of the selected item.
     * @param event mouse event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief Handle a mouse clicking and change a Z value of the selected item.
     * Emit a signal to push move action to Undo stack.
     * @param event mouse event
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief Handle a key event and control the selection of more than 1 item.
     * @param event key event
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Handle a key event and control the selection of more than 1 item.
     * @param event key event
     */
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    bool multSelect; ///< multiple select flag
    QGraphicsItem *movingItem = nullptr; ///< moved item
    QPointF oldPos; ///< initial position of a moved item

};

#endif //DIAGRAMS_EDITORSCENE_H

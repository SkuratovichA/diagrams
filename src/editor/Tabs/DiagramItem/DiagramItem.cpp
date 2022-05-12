// File: DiagramItem.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QInputEvent>

/**
 * A constructor.
 *
 * This constructor creates a text connected with its parent.
 *
 * @param p parent item
 * @param text text represented by a string
 * @param pos position of the text
 * @param flags flags for text that make it editable
 */
ClassTextAttr::ClassTextAttr(ClassDiagramItem *p, QString text, QPointF pos,
                             QFlags<Qt::TextInteractionFlag> flags)
        : QGraphicsTextItem(text, p) {
    setTextInteractionFlags(flags);
    setPos(pos);
    _parent = p;
}

/**
 * A destructor.
 */
ClassTextAttr::~ClassTextAttr() {

}

/**
 * Handle an event of the text editing for object attributes to align it.
 * In the case of entering "Key_Enter" the text editing is ended.
 *
 * @param event key event
 */
void ClassTextAttr::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        setPlainText(toPlainText().remove('\n'));
        clearFocus();
        return;
    }

    qreal maxLen = 0;

    for (auto item: parent()->attrs()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }
    for (auto item: parent()->methods()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }

    if (maxLen + 30 < parent()->width()) {
        maxLen = parent()->width() - 30;
    } else if (maxLen + 20 > parent()->width()) {
        maxLen = parent()->width() + 20;
    } else {
        return;
    }

    parent()->setRect(0, 0, maxLen, parent()->height());
    for (auto item: parent()->attrsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    for (auto item: parent()->methodsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    parent()->setWidth(maxLen);
    parent()->setRowWidth(maxLen);

    qreal midW = parent()->_head->boundingRect().width();
    qreal midO = parent()->width();
    parent()->_head->setPos((midO - midW) / 2, -40);
    for (auto x: this->parent()->connections()) {
        x->trackNodes();
    }
}

/**
 * A constructor.
 *
 * This constructor creates a text connected with its parent.
 *
 * @param parent parent item
 * @param flags flags for text that make it editable
 * @param pos position of the text
 * @param str text represented by a string
 */
NameObject::NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, QPointF pos, QString str)
        : QGraphicsTextItem(str, parent) {
    _parent = parent;
    setPos(pos);
    setFont(QFont("Courier", 20));
    setTextInteractionFlags(flags);
    topLevelItem();
}

/**
 * Handle an event of the text editing for object name to align it.
 * In the case of entering "Key_Enter" the text editing is ended.
 *
 * @param event key event
 */
void NameObject::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        setPlainText(toPlainText().remove('\n'));
        clearFocus();
        return;
    }
    auto *tmp1 = dynamic_cast<ClassDiagramItem *>(parent());
    auto *tmp2 = dynamic_cast<SequenceDiagramItem *>(parent());
    qreal midO = tmp1 == nullptr ? tmp2->width() : tmp1->width();
    qreal midW = boundingRect().width();
    //qDebug() << "tmp1 has coordinates: " << tmp1->x() << tmp1->y();
    setPos((midO - midW) / 2, -40);
}
// File: DiagramItem.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QInputEvent>


ClassTextAttr::ClassTextAttr(ClassDiagramItem *p, QString text, QPointF pos,
                             QFlags<Qt::TextInteractionFlag> flags)
        : QGraphicsTextItem(text, p) {
    setTextInteractionFlags(flags);
    setPos(pos);
    _parent = p;
}

ClassTextAttr::~ClassTextAttr() {}

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

NameObject::NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, QPointF pos, QString str)
        : QGraphicsTextItem(str, parent) {
    _parent = parent;
    setPos(pos);
    setFont(QFont("Courier", 20));
    setTextInteractionFlags(flags);
    topLevelItem();
}

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
    setPos((midO - midW) / 2, -40);
}
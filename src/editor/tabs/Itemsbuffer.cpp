//
// Created by shchepa on 4.5.22.
//

#include "Itemsbuffer.h"


ItemsBuffer::ItemsBuffer() {

}

ItemsBuffer::~ItemsBuffer() {
    qDebug() << "I was called, without loop!";
    for (auto x : classItems()) {
        qDebug() << "I was called!";
        delete x;
    }
}

//void ItemsBuffer::createClassItems() {
//    return;
//}

void ItemsBuffer::fillClassItems(ClassDiagramItem *item) {
    classParams *ptr;
    QList<QString> methods;
    QList<QString> attrs;

    for (auto x : item->methods()) {
        if (x->toPlainText() == "_METHODS_")  {
            continue;
        }

        methods.push_back(x->toPlainText());
    }

    for (auto x : item->attrs()) {
        attrs.push_back(x->toPlainText());
    }

    ptr = new classParams(item->x() + 40, item->y() + 40, 1.0,
                          item->_head->toPlainText(), item->color(),
                          item->width(), item->height(), attrs, methods);
    pushClassItem(ptr);
}

void ItemsBuffer::fillActorItems(ActorDiagramItem *item) {
    actorParams *ptr;

    ptr = new actorParams(item->x() + 40, item->y() + 40, 1.0,
                          item->_head->toPlainText(), item->color(),
                          item->width(), item->height());
    pushActorItem(ptr);
}

void ItemsBuffer::clearBuffer() {
    deleteClassItems();
    deleteActorItems();
}
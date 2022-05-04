//
// Created by shchepa on 4.5.22.
//

#include "Itemsbuffer.h"


ItemsBuffer::ItemsBuffer() {

}

void ItemsBuffer::createClassItems() {
    return;
}

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

    ptr = new classParams(item->x() + 40, item->y() + 40, 1.0, attrs, methods, item->_head->toPlainText());
    pushClassItem(ptr);
}

void ItemsBuffer::clearBuffer() {
    deleteClassItems();
}
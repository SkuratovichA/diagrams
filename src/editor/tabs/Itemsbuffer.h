//
// Created by shchepa on 4.5.22.
//

#ifndef DIAGRAMS_ITEMSBUFFER_H
#define DIAGRAMS_ITEMSBUFFER_H

#include "diagram/DiagramItem.h"
#include "fillItems.h"

class ItemsBuffer {
public:
    ItemsBuffer();

    void createClassItems();
    void fillClassItems(ClassDiagramItem *item);
    void clearBuffer();

    QList<classParams *> classItems() const {
        return _classItems;
    }

    QList<ClassConnectionItem *> connectionItems() const {
        return _connectionItems;
    }

    QList<ActorDiagramItem *> actorItems() const {
        return _actorItems;
    }

    QList<ActorConnectionItem *> messageItems() const {
        return _messageItems;
    }

    void pushClassItem(classParams *item) {
        _classItems.push_back(item);
    }

    void deleteClassItems() {
        for (auto x : _classItems) {
            _classItems.pop_front();
            qDebug() << "delete item from buffer copy/paste" << x;
            delete x;
        }
    }

private:
    QList<classParams *> _classItems;
    QList<ClassConnectionItem *> _connectionItems;
    QList<ActorDiagramItem *> _actorItems;
    QList<ActorConnectionItem *> _messageItems;
};


#endif //DIAGRAMS_ITEMSBUFFER_H

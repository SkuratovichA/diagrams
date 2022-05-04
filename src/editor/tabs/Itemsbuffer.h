//
// Created by shchepa on 4.5.22.
//

#ifndef DIAGRAMS_ITEMSBUFFER_H
#define DIAGRAMS_ITEMSBUFFER_H

#include "diagram/DiagramItem.h"

class ItemsBuffer {
public:
    ItemsBuffer();

    void createClassItems();

    QList<ClassDiagramItem *> classItems() const {
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

private:
    QList<ClassDiagramItem *> _classItems;
    QList<ClassConnectionItem *> _connectionItems;
    QList<ActorDiagramItem *> _actorItems;
    QList<ActorConnectionItem *> _messageItems;
};


#endif //DIAGRAMS_ITEMSBUFFER_H

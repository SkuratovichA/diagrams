//
// Created by shchepa on 4.5.22.
//

#ifndef DIAGRAMS_ITEMSBUFFER_H
#define DIAGRAMS_ITEMSBUFFER_H

#include "../../DiagramItem/DiagramItem.h"
#include "../../FillItems/ObjectParams.h"

class ItemsBuffer {
public:
    ItemsBuffer();
    ~ItemsBuffer();

    //void createClassItems();
    void fillClassItems(ClassDiagramItem *item);
    void fillActorItems(SequenceDiagramItem *item);
    void clearBuffer();
    void addClassItems(Class cls);

    QList<classParams *> classItems() const {
        return _classItems;
    }

    QList<ClassConnectionItem *> connectionItems() const {
        return _connectionItems;
    }

    QList<actorParams *> sequenceItems() const {
        return _actorItems;
    }

    QList<SequenceConnectionItem *> messageItems() const {
        return _messageItems;
    }

    void pushClassItem(classParams *item) {
        _classItems.push_back(item);
    }

    void pushActorItem(actorParams *item) {
        _actorItems.push_back(item);
    }

    void deleteClassItems() {
        for (auto x : _classItems) {
            _classItems.pop_front();
            delete x;
        }
    }

    void deleteActorItems() {
        for (auto x : _actorItems) {
            _actorItems.pop_front();
            delete x;
        }
    }

private:
    QList<classParams *> _classItems;
    QList<ClassConnectionItem *> _connectionItems;
    QList<actorParams *> _actorItems;
    QList<SequenceConnectionItem *> _messageItems;
};


#endif //DIAGRAMS_ITEMSBUFFER_H

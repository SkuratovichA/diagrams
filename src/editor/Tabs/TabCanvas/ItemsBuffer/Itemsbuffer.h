// File: Itemsbuffer.h
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#ifndef DIAGRAMS_ITEMSBUFFER_H
#define DIAGRAMS_ITEMSBUFFER_H

#include "../../FillItems/ObjectParams.h"

class ClassDiagramItem;
class SequenceDiagramItem;
class ClassConnectionItem;
class SequenceConnectionItem;
class ClassInterfaceItem;

class ItemsBuffer {
public:
    ItemsBuffer();
    ~ItemsBuffer();

    void clearBuffer();

    void addActorItems(Actor act);
    void fillActorItems(SequenceDiagramItem *item);

    void addClassItems(Class cls);
    void fillClassItems(ClassDiagramItem *item);

    void addRelationItems(ClassDiagramConnectionRepresentation conct);
    void fillRelationItems(ClassConnectionItem *item);

    void addMessageItems(Action action);
    void fillMessageItems(SequenceConnectionItem *item);

    void addInterfaceItems(Interface inter);
    void fillInterfaceItems(ClassInterfaceItem *item);

    QList<classParams *> classItems() {
        return _classItems;
    }

    QList<relationsParams *> relationItems() {
        return _relationItems;
    }

    QList<SequenceDiagramItemParameters *> sequenceItems() {
        return _actorItems;
    }

    QList<messageParams *> messageItems() {
        return _messageItems;
    }

    QList<InterfaceParams *> interfaceItems() {
        return _interfaceItems;
    }

    void pushClassItem(classParams *item) {
        _classItems.push_back(item);
    }

    void pushActorItem(SequenceDiagramItemParameters *item) {
        _actorItems.push_back(item);
    }

    void pushRelationItem(relationsParams *item) {
        _relationItems.push_back(item);
    }

    void pushMessageItem(messageParams *item) {
        _messageItems.push_back(item);
    }

    void pushInterfaceItem(InterfaceParams *item) {
        _interfaceItems.push_back(item);
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

    void deleteRelationItems() {
        for (auto x : _relationItems) {
            _relationItems.pop_front();
            delete x;
        }
    }

    void deleteMessageItems() {
        for (auto x : _messageItems) {
            _messageItems.pop_front();
            delete x;
        }
    }

    void deleteInterfaceItems() {
        for (auto x : _interfaceItems) {
            _interfaceItems.pop_front();
            delete x;
        }
    }


private:
    QList<classParams *> _classItems;
    QList<relationsParams *> _relationItems;
    QList<SequenceDiagramItemParameters *> _actorItems;
    QList<messageParams *> _messageItems;
    QList<InterfaceParams *> _interfaceItems;
};


#endif //DIAGRAMS_ITEMSBUFFER_H

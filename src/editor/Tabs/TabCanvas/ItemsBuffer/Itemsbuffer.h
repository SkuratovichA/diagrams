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

/**
 * Buffer for a clipboard
 */
class ItemsBuffer {
public:
    /**
     * @brief A Constructor
     */
    ItemsBuffer();

    /**
     * @brief A Destructor
     */
    ~ItemsBuffer();

    /**
     * @brief Clear all lists with elements from the scene.
     */
    void clearBuffer();

    /**
     * @brief Fill the structure SequenceDiagramItemParameters from json to another structure
     * for the next object created on the scene.
     * @param act structure with the data
     */
    void addActorItems(Actor act);

    /**
     * @brief Load the created parameters to the object
     * @param item object to load created the parameters to.
     */
    void fillActorItems(SequenceDiagramItem *item);

    /**
     * @brief Fill the structure ClassDiagramItemParameters from json to another structure
     * for the next object created on the scene.
     * @param cls structure with the data
     */
    void addClassItems(Class cls);

    /**
     * @brief Load the created parameters to the object
     * @param item object to load created parameters in to.
     */
    void fillClassItems(ClassDiagramItem *item);

    /**
     * @brief Fill the structure relationParams from json to another structure
     * for the next object created on the scene.
     * @param conct structure with the data
     */
    void addRelationItems(ClassDiagramConnectionRepresentation conct);

    /**
     * @brief Load the created parameters to the object
     * @param item object to load created parameters in to.
     */
    void fillRelationItems(ClassConnectionItem *item);

    /**
     * @brief Create a message from an action
     * @param action
     */
    void addMessageItems(Action action);

    /**
     * @brief Fill a SequenecConnectionItem from previously cerated parameters
     * @param item an object to fill with new parameters
     */
    void fillMessageItems(SequenceConnectionItem *item);

    /**
     * @brief Getter.
     * @return _classItems
     */
    QList<ClassDiagramItemParameters *> classItems() {
        return _classItems;
    }

    /**
     * @brief Getter.
     * @return _relationItems
     */
    QList<ClassDiagramConnectionParams *> relationItems() {
        return _relationItems;
    }

    /**
     * @brief Getter.
     * @return _actorItems
     */
    QList<SequenceDiagramItemParameters *> sequenceItems() {
        return _actorItems;
    }

    /**
     * @brief Getter.
     * @return _messageItems
     */
    QList<messageParams *> messageItems() {
        return _messageItems;
    }

    /**
     * @brief Add class diagram parameters to the list with "classes"
     * @param item New item
     */
    void pushClassItem(ClassDiagramItemParameters *item) {
        _classItems.push_back(item);
    }

    /**
     * @brief Add class diagram parameters to the list with sequence diagram item
     * @param item New item
     */
    void pushActorItem(SequenceDiagramItemParameters *item) {
        _actorItems.push_back(item);
    }

    /**
     * @brief Add class connection parameters to the list _relationItems with these parameters
     * @param item new item
     */
    void pushRelationItem(ClassDiagramConnectionParams *item) {
        _relationItems.push_back(item);
    }

    /**
     * @brief Add messages to the list with the messages _messageItems
     * @param item new item
     */
    void pushMessageItem(messageParams *item) {
        _messageItems.push_back(item);
    }

    /**
     * @brief Clear _classItems
     */
    void deleteClassItems() {
        for (auto x : _classItems) {
            _classItems.pop_front();
            delete x;
        }
    }

    /**
     * @brief Clear _actorItems
     */
    void deleteActorItems() {
        for (auto x : _actorItems) {
            _actorItems.pop_front();
            delete x;
        }
    }

    /**
     * @brief Clear _relationItems
     */
    void deleteRelationItems() {
        for (auto x : _relationItems) {
            _relationItems.pop_front();
            delete x;
        }
    }

    /**
     * @brief Clear _messageItems
     */
    void deleteMessageItems() {
        for (auto x : _messageItems) {
            _messageItems.pop_front();
            delete x;
        }
    }

private:
    QList<ClassDiagramItemParameters *> _classItems; ///< list with the parameters to create classs diagram objects
    QList<ClassDiagramConnectionParams *> _relationItems; ///< list with the parameters to create relations between classes
    QList<SequenceDiagramItemParameters *> _actorItems; ///< list with the parameters to create sequence diagram objects
    QList<messageParams *> _messageItems; ///< list with the messages to create objects of SequenceConnectionItem class
};

#endif //DIAGRAMS_ITEMSBUFFER_H

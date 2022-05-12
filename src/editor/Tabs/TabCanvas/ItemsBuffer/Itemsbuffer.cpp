// File: Itemsbuffer.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "Itemsbuffer.h"
#include "../../DiagramItem/DiagramItem.h"

/**
 * A constructor.
 */
ItemsBuffer::ItemsBuffer() {

}

/**
 * A destructor.
 */
ItemsBuffer::~ItemsBuffer() {
    clearBuffer(); // FIXME if segfault delete
}

/**
 * Fill the structure relationParams from json to another structure
 * for the next object created on the scene.
 *
 * @param conct structure with the data
 */
void ItemsBuffer::addRelationItems(ClassDiagramConnectionRepresentation conct) {
    ClassDiagramConnectionParams *prm = new ClassDiagramConnectionParams(QString::fromStdString(conct.leftObj),
                                                                         QString::fromStdString(conct.leftNum),
                                                                         QString::fromStdString(conct.rightObj),
                                                                         QString::fromStdString(conct.rightNum),
                                                                         QString::fromStdString(conct.msg),
                                                                         conct.arrow, conct.order);

    pushRelationItem(prm);
}

/**
 *
 * @param item
 */
void ItemsBuffer::fillRelationItems(ClassConnectionItem *item) {
    ClassDiagramConnectionParams *ptr = new ClassDiagramConnectionParams(
            item->nodeFrom()->name(), item->getLeftNum()->toPlainText(),
            item->nodeTo()->name(), item->getRightNum()->toPlainText(),
            item->getMsg()->toPlainText(), item->connectionType(), item->order());

    pushRelationItem(ptr);
}

/**
 * Fill the structure ClassDiagramItemParameters from json to another structure
 * for the next object created on the scene.
 *
 * @param cls structure with the data
 */
void ItemsBuffer::addClassItems(Class cls) {
    ClassDiagramItemParameters *prm;
    QList<QString> methods;
    QList<QString> attrs;

    for (auto x: cls.attrs) {
        attrs.push_back(QString::fromStdString(x.perm + " " + x.type + " " + x.name));
    }

    for (auto x: cls.methods) {
        methods.push_back(QString::fromStdString(x.perm + " " + x.type + " " + x.name));
    }

    prm = new ClassDiagramItemParameters(cls.coords[0], cls.coords[1], QString::fromStdString(cls.name),
                                         QColor(cls.color.r, cls.color.g, cls.color.b, cls.color.a),
                                         cls.width, cls.height, attrs, methods);

    pushClassItem(prm);
}

/**
 *
 * @param item
 */
void ItemsBuffer::fillClassItems(ClassDiagramItem *item) {
    ClassDiagramItemParameters *ptr;
    QList<QString> methods;
    QList<QString> attrs;

    for (auto x: item->methods()) {
        if (x->toPlainText() == "METHODS") {
            continue;
        }

        methods.push_back(x->toPlainText());
    }

    for (auto x: item->attrs()) {
        attrs.push_back(x->toPlainText());
    }

    ptr = new ClassDiagramItemParameters(item->x() + 40, item->y() + 40,
                                         item->_head->toPlainText(), item->color(),
                                         item->width(), item->height(), attrs,
                                         methods);
    pushClassItem(ptr);
}

/**
 * Fill the structure SequenceDiagramItemParameters from json to another structure
 * for the next object created on the scene.
 *
 * @param act structure with the data
 */
void ItemsBuffer::addActorItems(Actor act) {
    SequenceDiagramItemParameters *a;

    a = new SequenceDiagramItemParameters(act.coords[0], act.coords[1],
                                          QString::fromStdString(act.name),
                                          QColor(act.color.r, act.color.g, act.color.b, act.color.a));

    pushActorItem(a);
}

/**
 *
 * @param item
 */
void ItemsBuffer::fillMessageItems(SequenceConnectionItem *item) {
    messageParams *ptr;

    // GET ID of item
    ptr = new messageParams(item->x(), item->y(),
                            item->getText()->toPlainText(),
                            item->nodeFrom()->name(), item->nodeTo()->name(),
                            item->type());
    pushMessageItem(ptr);
}

void ItemsBuffer::addMessageItems(Action action) {
    messageParams *a;

    a = new messageParams(action.coords[0], action.coords[1],
                          QString::fromStdString(action.msg),
                          QString::fromStdString(action.from),
                          QString::fromStdString(action.to),
                          action.type/*, action.fromId, action.toId*/);

    pushMessageItem(a);
}

/**
 *
 * @param item
 */
void ItemsBuffer::fillActorItems(SequenceDiagramItem *item) {
    SequenceDiagramItemParameters *ptr;

    ptr = new SequenceDiagramItemParameters(item->x(), item->y(),
                                            item->_head->toPlainText(), item->color());
    // get id of item
    //qDebug() << reinterpret_cast<uintptr_t>(item) % 10000 << "addr as int";
    ptr = new SequenceDiagramItemParameters(item->x(), item->y(),
                                            item->_head->toPlainText(),
                                            item->color());
    pushActorItem(ptr);
}

/**
 * Clear all lists with elements from the scene.
 */
void ItemsBuffer::clearBuffer() {
    deleteClassItems();
    deleteActorItems();
    deleteRelationItems();
}
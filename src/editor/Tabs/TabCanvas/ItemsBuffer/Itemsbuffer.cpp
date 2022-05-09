//
// Created by shchepa on 4.5.22.
//

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
 * Fill the structure from json to another structure for the next object created on the scene.
 *
 * @param conct structure with the data
 */
void ItemsBuffer::addRelationItems(Conct conct) {
    relationsParams *prm = new relationsParams(QString::fromStdString(conct.leftObj),
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
    relationsParams *ptr =  new relationsParams(
            item->nodeFrom()->name(), item->getLeftNum()->toPlainText(),
            item->nodeTo()->name(), item->getRightNum()->toPlainText(),
            item->getMsg()->toPlainText(), item->connectionType(), item->order());

    pushRelationItem(ptr );
}

/**
 *
 * @param cls
 */
void ItemsBuffer::addClassItems(Class cls) {
    classParams *prm;
    QList<QString> methods;
    QList<QString> attrs;

    for (auto x : cls.attrs) {
        attrs.push_back(QString::fromStdString(x.perm + " " + x.type + " " + x.name));
    }

    for (auto x : cls.methods) {
        methods.push_back(QString::fromStdString(x.perm + " " + x.type + " " + x.name));
    }

    prm = new classParams(cls.coords[0], cls.coords[1], QString::fromStdString(cls.name),
                          QColor(cls.color.r, cls.color.g, cls.color.b, cls.color.a),
                          cls.width, cls.height, attrs, methods);

    pushClassItem(prm);
}

void ItemsBuffer::fillClassItems(ClassDiagramItem *item) {
    classParams *ptr;
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

    ptr = new classParams(item->x() + 40, item->y() + 40,
                          item->_head->toPlainText(), item->color(),
                          item->width(), item->height(), attrs, methods);
    pushClassItem(ptr);
}

/**
 *
 * @param act
 */
void ItemsBuffer::addActorItems(Actor act) {
    actorParams *a;

    a = new actorParams(act.coords[0], act.coords[1],
                        QString::fromStdString(act.name),
                        QColor(act.color.r, act.color.g, act.color.b, act.color.a));

    pushActorItem(a);
}

/**
 *
 * @param item
 */
void ItemsBuffer::fillActorItems(SequenceDiagramItem *item) {
    actorParams *ptr;

    ptr = new actorParams(item->x(), item->y(),
                          item->_head->toPlainText(), item->color());
    pushActorItem(ptr);
}

/**
 *
 */
void ItemsBuffer::clearBuffer() {
    deleteClassItems();
    deleteActorItems();
    deleteRelationItems();
}
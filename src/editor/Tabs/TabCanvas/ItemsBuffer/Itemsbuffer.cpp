/** @file ItemBuffer.cpp
 * @author Shchapaniak Andrei <xshcha00\@vutbr.cz>
 * @date 07.05.2022
 */

#include "Itemsbuffer.h"
#include "../../DiagramItem/DiagramItem.h"

ItemsBuffer::ItemsBuffer() {

}

ItemsBuffer::~ItemsBuffer() {
    clearBuffer();
}

void ItemsBuffer::addRelationItems(ClassDiagramConnectionRepresentation conct) {
    relationsParams *prm = new relationsParams(QString::fromStdString(conct.leftObj),
                                               QString::fromStdString(conct.leftNum),
                                               QString::fromStdString(conct.rightObj),
                                               QString::fromStdString(conct.rightNum),
                                               QString::fromStdString(conct.msg),
                                               conct.arrow, conct.order);

    pushRelationItem(prm);
}

void ItemsBuffer::fillRelationItems(ClassConnectionItem *item) {
    relationsParams *ptr = new relationsParams(item->nodeFrom()->name(),
                                               item->getLeftNum()->toPlainText(),
                                               item->nodeTo()->name(),
                                               item->getRightNum()->toPlainText(),
                                               item->getMsg()->toPlainText(),
                                               item->connectionType(),
                                               item->order());

    pushRelationItem(ptr);
}

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
    prm = new ClassDiagramItemParameters(cls.coords[0], cls.coords[1],
                                         QString::fromStdString(cls.name),
                                         QColor(cls.color.r, cls.color.g, cls.color.b, cls.color.a),
                                         cls.width,
                                         cls.height,
                                         attrs,
                                         methods);

    pushClassItem(prm);
}

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

void ItemsBuffer::addActorItems(Actor act) {
    SequenceDiagramItemParameters *a;

    a = new SequenceDiagramItemParameters(act.coords[0],
                                          act.coords[1],
                                          QString::fromStdString(act.name),
                                          QColor(act.color.r, act.color.g, act.color.b, act.color.a));

    pushActorItem(a);
}

void ItemsBuffer::fillMessageItems(SequenceConnectionItem *item) {
    messageParams *ptr;

    // GET ID of item
    ptr = new messageParams(item->x(), item->y(),
                            item->getText()->toPlainText(),
                            item->nodeFrom()->name(),
                            item->nodeTo()->name(),
                            item->type());
    pushMessageItem(ptr);
}

void ItemsBuffer::addMessageItems(Action action) {
    messageParams *a;

    a = new messageParams(action.coords[0], action.coords[1],
                          QString::fromStdString(action.msg),
                          QString::fromStdString(action.from),
                          QString::fromStdString(action.to),
                          action.type);

    pushMessageItem(a);
}

void ItemsBuffer::fillActorItems(SequenceDiagramItem *item) {
    SequenceDiagramItemParameters *ptr;

    // get id of item
    ptr = new SequenceDiagramItemParameters(item->x(), item->y(),
                                            item->_head->toPlainText(),
                                            item->color());
    pushActorItem(ptr);
}

void ItemsBuffer::clearBuffer() {
    deleteClassItems();
    deleteActorItems();
    deleteRelationItems();
}
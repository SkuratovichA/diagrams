//
// Created by shchepa on 4.5.22.
//

#include "Itemsbuffer.h"

ItemsBuffer::ItemsBuffer() {

}

ItemsBuffer::~ItemsBuffer() {
    for (auto x : classItems()) {
        delete x;
    }
}

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

void ItemsBuffer::addRelationItems(Conct conct) {
    relationsParams *prm = new relationsParams(QString::fromStdString(conct.left_obj),
                                               QString::fromStdString(conct.left_num),
                                               QString::fromStdString(conct.right_obj),
                                               QString::fromStdString(conct.right_num),
                                               QString::fromStdString(conct.msg),
                                               conct.arrow);

    pushRelationItem(prm);
}

void ItemsBuffer::fillRelationItems(ClassConnectionItem *item) {
    relationsParams *ptr =  new relationsParams(
            item->nodeFrom()->name(), item->getLeftNum()->toPlainText(),
            item->nodeTo()->name(), item->getRightNum()->toPlainText(),
            item->getMsg()->toPlainText(), item->connectionType());

    pushRelationItem(ptr );
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

void ItemsBuffer::fillActorItems(SequenceDiagramItem *item) {
    actorParams *ptr;

    ptr = new actorParams(item->x() + 40, item->y() + 40,
                          item->_head->toPlainText(), item->color(),
                          item->width(), item->height());
    pushActorItem(ptr);
}

void ItemsBuffer::clearBuffer() {
    deleteClassItems();
    deleteActorItems();
}
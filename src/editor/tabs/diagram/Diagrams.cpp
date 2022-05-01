// File: Diagrams.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 30.04.2022


#include "Diagrams.h"
#include "../object.h"

// delete after
Actor* Diagram::addEntity(QGraphicsScene *scene) {
    Actor *actor = new Actor(100,100);
    scene->addItem(actor);
    return actor;
}

// delete after
Message* Diagram::addConnection(QGraphicsScene *scene) {
    Message *msg = new Message(100,100);
    scene->addItem(msg);
    return msg;
}


/**
 *
 * @param parent
 */
SequenceDiagram::SequenceDiagram(QObject *parent) {
}

/**
 *
 */
SequenceDiagram::~SequenceDiagram() {
}

void SequenceDiagram::sendToBack()  {

}

void SequenceDiagram::sendToFront() {
}

/**
 *
 */
Actor* SequenceDiagram::addEntity(QGraphicsScene *scene) {
    Actor *actor = new Actor(100,100);
    scene->addItem(actor);
    return actor;
}

/**
 *
 */
Message* SequenceDiagram::addConnection(QGraphicsScene *scene) {
    Message *msg = new Message(100,100);
    scene->addItem(msg);
    return msg;
}

void SequenceDiagram::setProperties() {
}


/***************************************/
/**
 *
 * @param parent
 */
ClassDiagram::ClassDiagram(QObject *parent) {

}

/**
 *
 */
ClassDiagram::~ClassDiagram() {

}

/**
 *
 */
void ClassDiagram::sendToBack() {
}

void ClassDiagram::sendToFront() {
}

/**
 *
 */
Class* ClassDiagram::addEntity(QGraphicsScene *scene) {
    Class *class_ = new Class(100,100);
    scene->addItem(class_);
    return class_;
}

/**
 *
 */
Relation* ClassDiagram::addConnection(QGraphicsScene *scene) {
    Relation *relation = new Relation(100,100);
    scene->addItem(relation);
    return relation;
}

/**
 *
 */
void ClassDiagram::setProperties() {
}

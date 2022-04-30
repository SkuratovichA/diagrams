// File: Diagrams.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 30.04.2022


#include "Diagrams.h"
#include "../object.h"

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
Object* SequenceDiagram::addEntity() {
    return nullptr;
}

/**
 *
 */
Object* SequenceDiagram::addConnection() {
    return nullptr;
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
Object* ClassDiagram::addEntity() {
    return nullptr;
}

/**
 *
 */
Object* ClassDiagram::addConnection() {
    return nullptr;
}

/**
 *
 */
void ClassDiagram::setProperties() {
}

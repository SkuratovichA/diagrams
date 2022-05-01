// File: Diagrams.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 30.04.2022

#ifndef DIAGRAMS_DIAGRAMS_H
#define DIAGRAMS_DIAGRAMS_H


#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QPushButton>

#include "../object.h"

class TabCanvas;

/**
 *
 */
class Diagram : public QMainWindow  {
Q_OBJECT

public:
    // TODO: reimplement me inside of classes and make me return nullptr.
    Actor* addEntity(QGraphicsScene *scene);

    // TODO: reimplement me inside of classes and make me return nullptr.
    Message* addConnection(QGraphicsScene *scene);

    virtual void setProperties() {};
    virtual void sendToBack() {};
    virtual void sendToFront() {};
protected:
    /* do we really have to add here all the stuff from the protected: of Scene */
};


class SequenceDiagram : public Diagram {

public:
    explicit SequenceDiagram(QObject *parent = nullptr);
    ~SequenceDiagram();

public:
    Actor* addEntity(QGraphicsScene *scene);
    Message* addConnection(QGraphicsScene *scene);
    void setProperties() override;
    void sendToBack() override;
    void sendToFront() override;
};


class ClassDiagram : public Diagram {

public:
    explicit ClassDiagram(QObject *parent = nullptr);
    ~ClassDiagram();

public:
    Class* addEntity(QGraphicsScene *scene);
    Relation* addConnection(QGraphicsScene *scene);
    void setProperties() override;
    void sendToBack() override;
    void sendToFront() override;
};

#endif //DIAGRAMS_DIAGRAMS_H

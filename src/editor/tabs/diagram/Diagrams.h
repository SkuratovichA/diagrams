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
    virtual Object* addEntity() {
        Object *obj = new Object();
//    obj->setText("support in the future version of the project. Dont delete me please. I have kids and wife.");
        obj->setPos(QPoint(20, 20));

        obj->setSelected(true);
        return obj;
    };

    // TODO: reimplement me inside of classes and make me return nullptr.
    virtual Object* addConnection() {
        Object *obj = new Object();
//    obj->setText("support in the future version of the project. Dont delete me please. I have kids and wife.");
        obj->setPos(QPoint(20, 20));

        obj->setSelected(true);
        return obj;
    }

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
    Object* addEntity() override;
    Object* addConnection() override;
    void setProperties() override;
    void sendToBack() override;
    void sendToFront() override;
};


class ClassDiagram : public Diagram {

public:
    explicit ClassDiagram(QObject *parent = nullptr);
    ~ClassDiagram();

public:
    Object* addEntity() override;
    Object* addConnection() override;
    void setProperties() override;
    void sendToBack() override;
    void sendToFront() override;
};

#endif //DIAGRAMS_DIAGRAMS_H
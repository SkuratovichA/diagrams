// File: Connections.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#ifndef DIAGRAMS_CONNECTIONS_H
#define DIAGRAMS_CONNECTIONS_H

#includen <QGraphicsLineItem>

#include "DiagramItem.h"


/**
 *
 */
class ClassConnectionItem : public QGraphicsLineItem {
public:
    enum ClassConnectionType {
        Aggregation, Composition, Generalization, Association
    };

    ClassConnectionItem(
            ClassDiagramItem *fromNode,
            QVector<ClassDiagramItem *> toNode,
            ClassConnectionType connectionType);

    ~ClassConnectionItem();

    ClassDiagramItem *fromNode() const;

    QVector<ClassDiagramItem *> toNode() const;

    void setColor(const QColor &color);

    QColor color() const;

    void trackNodes();

private:
    ClassDiagramItem *nodeFrom;
    QVector<ClassDiagramItem *> nodeTo;
    QLineF connectionLine;
};

/**
 *
 */
class ActorConnectionItem : public QGraphicsLineItem {
public:
    enum ActorConnectionType {
        Requect, Responce
    };

    ActorConnectionItem(
            ActorDiagramItem *fromNode,
            ActorDiagramItem *toNode,
            ActorConnectionType connectionType);

    ~ActorConnectionItem();

    void setColor(const QColor &color);

    QColor color() const;

    void trackNodes();

    ActorDiagramItem *fromNode() const;

    ActorDiagramItem *toNode() const;

private:
    ActorDiagramItem *nodeFrom;
    ActorDiagramItem *nodeTo;
    QLineF connectionLine;
};


#endif //DIAGRAMS_CONNECTIONS_H

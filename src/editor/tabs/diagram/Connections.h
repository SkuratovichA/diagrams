// File: Connections.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#ifndef DIAGRAMS_CONNECTIONS_H
#define DIAGRAMS_CONNECTIONS_H

#include <QGraphicsLineItem>

class ClassDiagramItem;

class ActorDiagramItem;

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
            ClassDiagramItem *toNode,
            ClassConnectionType connectionType);

    ~ClassConnectionItem();

    ClassDiagramItem *fromNode() const;
    ClassDiagramItem *toNode() const;
    void setColor(const QColor &color);
    QColor color() const;

    void trackNodes();
private:
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    ClassDiagramItem *nodeFrom;
    ClassDiagramItem *nodeTo;
    QLineF connectionLine;
};

/**
 *
 */
class ActorConnectionItem : public QGraphicsLineItem {
public:
    enum ActorConnectionType {
        Request, Response
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

// File: Connections.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#ifndef DIAGRAMS_CONNECTIONS_H
#define DIAGRAMS_CONNECTIONS_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

class ClassDiagramItem;

class ActorDiagramItem;

/**
 *
 */
class ClassConnectionItem : public QGraphicsLineItem {
    /** Constructor, destructor
     */
public:
    enum ClassConnectionType {
        Aggregation, Composition, Generalization, Association, Dependency
    };
    ClassConnectionItem(
            ClassDiagramItem *fromNode,
            ClassDiagramItem *toNode,
            ClassConnectionType connectionType,
            uint32_t order = 0,
            QColor color = QColor(50,45,50,100)
            );

    ~ClassConnectionItem();

    /** Public functions
     */
public:
    void setType(ClassConnectionType type) {
        _connectionType = type;
    }
    ClassConnectionType connectionType() const {
        return _connectionType;
    }
    ClassDiagramItem *fromNode() const;
    ClassDiagramItem *toNode() const;
    void trackNodes();

    void setColor(const QColor &color);
    QColor color() const;
    uint32_t order() const {
        return _order;
    }

    /** Protected methods
     */
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /** Private methods
     */
private:
    void drawLine(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    QPolygonF lineShaper() const;
    QPair<QPointF, QPointF> edgePoints() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /** Private attributes
     */
private:
    ClassDiagramItem *nodeFrom;
    ClassDiagramItem *nodeTo;
    QLineF connectionLine;
    ClassConnectionType _connectionType;
    QColor _color;
    uint32_t _order;
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

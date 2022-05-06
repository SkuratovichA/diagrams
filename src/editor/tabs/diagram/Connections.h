// File: Connections.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#ifndef DIAGRAMS_CONNECTIONS_H
#define DIAGRAMS_CONNECTIONS_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>


class ClassDiagramItem;
class SequenceDiagramItem;

class msgText;

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

    /** Getters, setters
     */
public:
    void setType(ClassConnectionType type) {
        _connectionType = type;
    }
    ClassConnectionType connectionType() const {
        return _connectionType;
    }
    ClassDiagramItem *nodeFrom() const {
        return _nodeFrom;
    }
    ClassDiagramItem *nodeTo() const {
        return _nodeTo;
    }
    QColor color() const {
        return _color;
    }
    uint32_t order() const {
        return _order;
    }
    void changeOrientation() {
        _orientation = !_orientation;
        trackNodes();
    }
    void setColor(const QColor &color) {
        _color = color;
    }

    QFlags<Qt::TextInteractionFlag> getFlags() {
        return Qt::TextInteractionFlag::TextEditable |
               Qt::TextInteractionFlag::TextSelectableByMouse |
               Qt::TextInteractionFlag::TextSelectableByKeyboard;
    }

    /** Public functions
     */
public:
    void trackNodes();

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
    bool _single;

    /** Private attributes
     */
private:
    ClassDiagramItem *_nodeFrom;
    ClassDiagramItem *_nodeTo;
    ClassConnectionType _connectionType;
    bool _orientation = true;
    QColor _color;
    uint32_t _order;
    msgText *msg;
};

/**
 *
 */
class ActorConnectionItem : public QGraphicsLineItem {
public:
    enum ActorConnectionType {
        Request, Response, Delete, Create
    };

    ActorConnectionItem(
            SequenceDiagramItem *fromNode,
            SequenceDiagramItem *toNode,
            ActorConnectionType connectionType);

    ~ActorConnectionItem();

    QColor color() const;

    void trackNodes();

    SequenceDiagramItem *fromNode() const;

    SequenceDiagramItem *toNode() const;
public:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    SequenceDiagramItem *nodeFrom;
    SequenceDiagramItem *nodeTo;
    QLineF connectionLine;
};

class msgText : public QGraphicsTextItem {
public:
    msgText(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y, QString str);
    //~msgText();

    QGraphicsItem *parent() {
        return _parent;
    }

protected:
    void keyReleaseEvent(QKeyEvent *event);

private:
    QGraphicsItem *_parent;
};

#endif //DIAGRAMS_CONNECTIONS_H

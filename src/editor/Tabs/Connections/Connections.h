// File: Connections.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#ifndef DIAGRAMS_CONNECTIONS_H
#define DIAGRAMS_CONNECTIONS_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include "../TabCanvas/ItemsBuffer/Itemsbuffer.h"

class ClassDiagramItem;

class SequenceDiagramItem;

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
            relationsParams *params,
            ClassConnectionType type,
            uint32_t order = 0,
            QColor color = QColor(50, 45, 50, 100)
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

    void setPosLeft(QPointF pos) {
        leftNum->setPos(pos);
    }

    void setPosRight(QPointF pos) {
        rightNum->setPos(pos);
    }

    msgText* getLeftNum() const {
        return leftNum;
    }

    msgText* getRightNum() const {
        return rightNum;
    }

    msgText* getMsg() const {
        return msg;
    }

    void msgSetPos(QPointF pos) {
        msg->setPos(pos);
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
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /** Private methods
     */
private:
    void drawLine(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    [[nodiscard]] QPolygonF lineShaper() const;
    [[nodiscard]] QPair<QPointF, QPointF> edgePoints() const ;
    [[nodiscard]] QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
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
    msgText *leftNum;
    msgText *rightNum;
};

/**
 *
 */
class SequenceConnectionItem : public QGraphicsLineItem {
public:
    enum SequenceConnectionType {
        Synchronous, Asynchronous, Reply, Create, Delete,
    };

public:
    SequenceConnectionItem(
            SequenceDiagramItem *fromNode,
            SequenceDiagramItem *toNode,
            SequenceConnectionType connectionType);

    ~SequenceConnectionItem();

public:
    QColor color() const;
    void trackNodes();

    SequenceDiagramItem *fromNode() const {
        return nodeFrom;
    };

    SequenceDiagramItem *toNode() const {
        return nodeTo;
    };

public:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    SequenceDiagramItem *nodeFrom;
    SequenceDiagramItem *nodeTo;
    QLineF connectionLine;
};

#endif //DIAGRAMS_CONNECTIONS_H

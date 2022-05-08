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
            QColor color = QColor(50, 45, 50, 100)
    );

    ~ClassConnectionItem();

    /** Getters, setters
     */
public:
    void setType(ClassConnectionType type) {
        _connectionType = type;
    }

    [[nodiscard]] ClassConnectionType connectionType() const {
        return _connectionType;
    }

    [[nodiscard]] ClassDiagramItem *nodeFrom() const {
        return _nodeFrom;
    }

    [[nodiscard]] ClassDiagramItem *nodeTo() const {
        return _nodeTo;
    }

    [[nodiscard]] QColor color() const {
        return _color;
    }

    [[nodiscard]] uint32_t order() const {
        return _order;
    }

    void changeOrientation() {
        _orientation = !_orientation;
        trackNodes();
    }

    void setColor(const QColor &color) {
        _color = color;
    }

    static QFlags<Qt::TextInteractionFlag> getFlags() {
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
    [[nodiscard]] QPair<QPointF, QPointF> edgePoints() const;
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
    [[nodiscard]] QColor color() const;
    void trackNodes();

    [[nodiscard]] SequenceDiagramItem *fromNode() const {
        return nodeFrom;
    };

    [[nodiscard]] SequenceDiagramItem *toNode() const {
        return nodeTo;
    };

public:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

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
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QGraphicsItem *_parent;
};

#endif //DIAGRAMS_CONNECTIONS_H

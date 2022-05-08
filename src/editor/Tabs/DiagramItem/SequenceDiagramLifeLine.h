// File: SequenceDiagramLifeLine.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022


#ifndef DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H
#define DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include "DiagramItem.h"

/** Implementation of a custom Life Line for an object (actor)
 */
class SequenceDiagramLifeLine : public QGraphicsLineItem {
    /** Constructor, Destructor
     */
public:
    SequenceDiagramLifeLine(SequenceDiagramItem *parent, qreal yFrom, qreal height);
    ~SequenceDiagramLifeLine() override;


public:
    void trackNodes();
    /** Getters, setters
     */
public:
    [[nodiscard]] qreal yFrom() const {
        return _yFrom;
    }

    [[nodiscard]] qreal height() const {
        return _height;
    }

    [[nodiscard]] const QList<QPair<qreal, qreal>> &activeRegions() const {
        return _activeRegions;
    }

    [[nodiscard]] qreal defaultActiveRegionLength() const {
        return _defaultActiveRegionLength;
    }

    [[nodiscard]] const QList<qreal> &synchronousPoints() const {
        return _synchronousPoints;
    }

    [[nodiscard]] bool isIsWaitingForResponce() const {
        return _isWaitingForResponce;
    }

public:
    void setYFrom(qreal yFrom) {
        _yFrom = yFrom;
    }

    void setYTo(qreal height) {
        _height = height;
    }

    void addActiveRegion(const QPair<qreal, qreal> activeRegions) {
        _activeRegions.push_back(activeRegions);
    }

    void addSynchronousPoint(const qreal synchronousPoint) {
        _synchronousPoints.push_back(synchronousPoint);
    }

    void setIsWaitingForResponce(bool isWaitingForResponce) {
        _isWaitingForResponce = isWaitingForResponce;
    }

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    [[nodiscard]] QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPolygonF lineShaper() const;

    qreal _yFrom = 0;
    qreal _height = 0;
    QLineF _line;
    SequenceDiagramItem *_parent;

    // Usually from/to, but in case to is -1, use default length.
    QList<QPair<qreal /*from*/, qreal /*to*/>> _activeRegions;
    qreal const _defaultActiveRegionLength = 20;

    // the first point is an entrypoint. Each point will switch
    // a boolean flag, and, hence, activate or diactivate a region.
    QList<qreal> _synchronousPoints;
    bool _isWaitingForResponce = false;
};

#endif //DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

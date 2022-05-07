// File: SequenceDiagramLifeLine.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022


#ifndef DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H
#define DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

class SequenceDiagramLifeLine : public QGraphicsLineItem {
    /** Constructor, Destructor
     */
public:
    SequenceDiagramLifeLine(qreal yFrom, qreal yTo);
    ~SequenceDiagramLifeLine();

    /** Getters, setters
     */
public:
    void setYFrom(qreal yFrom) {
        SequenceDiagramLifeLine::yFrom = yFrom;
    }

    void setYTo(qreal yTo) {
        SequenceDiagramLifeLine::yTo = yTo;
    }

    void setLlLength(qreal llLength) {
        LLLength = llLength;
    }

    void setActiveRegions(const QList<QPair<qreal, qreal>> &activeRegions) {
        SequenceDiagramLifeLine::activeRegions = activeRegions;
    }

    void setDefaultActiveRegionLength(qreal defaultActiveRegionLength) {
        SequenceDiagramLifeLine::defaultActiveRegionLength = defaultActiveRegionLength;
    }

    void setSynchronousPoints(const QList<qreal> &synchronousPoints) {
        SequenceDiagramLifeLine::synchronousPoints = synchronousPoints;
    }

    void setIsWaitingForResponce(bool isWaitingForResponce) {
        SequenceDiagramLifeLine::isWaitingForResponce = isWaitingForResponce;
    }

private:
    qreal yFrom = 0;
    qreal yTo = 0;
    qreal LLLength = 0;

    // Usually from/to, but in case to is -1, use default length.
    QList<QPair<qreal /*from*/, qreal /*to*/>> activeRegions;
    qreal defaultActiveRegionLength = 20;

    // the first point is an entrypoint. Each point will switch
    // a boolean flag, and, hence, activate or diactivate a region.
    QList<qreal> synchronousPoints;
    bool isWaitingForResponce = false;


};

#endif //DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

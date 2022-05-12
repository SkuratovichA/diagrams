// File: SequenceDiagramLifeLine.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022


#ifndef DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H
#define DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include "../DiagramItem.h"
//#include "../../Connections/Connections.h"

class SequenceConnectionItem;

using namespace Connections;

typedef QPair<ActorType, const SequenceConnectionItem *> actorConnectionPair_t;
typedef QList<actorConnectionPair_t> ActorTypeConnectionList_t;
typedef QPair<qreal, qreal> region_t;

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

    [[nodiscard]] qreal defaultActiveRegionLength() const {
        return _actRegLen;
    }

public:
    void setYFrom(qreal yFrom) {
        _yFrom = yFrom;
    }

    void setHeight(qreal height) {
        _height = height;
    }

//    void addActiveRegion(const SequenceConnectionItem *activeRegions) {
//        _activeRegions.push_back(activeRegions);
//    }
//
//    void addSynchronousPoint(const SequenceConnectionItem *synchronousPoint) {
//        _messages.push_back(synchronousPoint);
//    }
//
    /**
     * Usual functions.
     */
public:
    void addConnection(
            SequenceConnectionItem *connection,
            ActorType actorType
    );
    void removeConnection(const SequenceConnectionItem *y);

    /**
     * Private functions.
     */
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    [[nodiscard]] QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    [[nodiscard]] QPolygonF lineShaper() const;


    /** Merges all intervals
     *
     * @return merged intervals
     */
    void updateActiveRegions();

    /**
     *
     * @param a
     * @return
     */
    QList<region_t> getAsynchronousRegionsAsIntervals(ActorTypeConnectionList_t &a);


    /**
     * Create pairs in such a way, that for a <= b <= `max`
     * they will look like<a, max> or <a, b>, if such b exists.
     *
     * @param a
     * @return
     */
    QList<region_t> getSynchronousRegionsAsIntervals(ActorTypeConnectionList_t &a);

    /**
     *
     * @param a
     * @return
     */
    QList<qreal> getFirstCreateRegion() const;

    /**
     *
     * @param a
     * @return
     */
    QList<qreal> getLastDeleteRegion() const;

    /**
     * Private variables
     */
private:
    qreal _yFrom = 0;
    qreal _height = 1000; // TODO set height
    SequenceDiagramItem *_parent;

    /**
     * Usually, from/to, but in case to is -1, use default length.
     */
    ActorTypeConnectionList_t _activeRegions = ActorTypeConnectionList_t();
    ActorTypeConnectionList_t _async_replyMessages = ActorTypeConnectionList_t();

    QList<SequenceConnectionItem *> _createMessages = QList<SequenceConnectionItem *>();
    QList<SequenceConnectionItem *> _deleteMessages = QList<SequenceConnectionItem *>();

    QList<region_t> _mergedActiveRegions;

    /**
     * Constants.
     */
private:
    qreal const _actRegLen = 20.0;
    qreal _adjust = 10.0;

    qreal _verticalAgjust = 0.0; ///< number representing the difference of the absolute and a relative positions(parent)
    qreal const _heightAdjust = 20.0;
};

#endif //DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

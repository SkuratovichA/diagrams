/** @file SequenceDiagramLifeLine.h
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 04.05.2022
 */


#ifndef DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H
#define DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include "../DiagramItem.h"

class SequenceConnectionItem;

using namespace Connections;

typedef QPair<ActorType, const SequenceConnectionItem *> actorConnectionPair_t;
typedef QList<actorConnectionPair_t> ActorTypeConnectionList_t;
typedef QPair<qreal, qreal> region_t;

/** 
 * @brief Implementation of a custom Life Line for an object (actor)
 */
class SequenceDiagramLifeLine : public QGraphicsLineItem {
public:
    /**
     * @brief A constructor.
     * @param yFrom the starting position of the line.
     * @param height the height of the line.
     */
    SequenceDiagramLifeLine(SequenceDiagramItem *parent, qreal yFrom, qreal height);

    /**
     * A destructor.
     */
    ~SequenceDiagramLifeLine() override;

public:
    void trackNodes();

public:
    /**
     * @brief Getter. Returns a height of a lifeline
     * @return qreal _height
     */
    [[nodiscard]] qreal height() const {
        return _height;
    }

public:
    /**
     * @brief Add a connection to a list with connections
     * @param connection
     * @param actorType
     */
    void addConnection(
            SequenceConnectionItem *connection,
            ActorType actorType
    );

    /**
     * @brief Remove connectiono from a list with with connections
     * @param connection connection of class SequenceConnectionItem to remove
     */
    void removeConnection(const SequenceConnectionItem *connection);

private:
    /**
     * @brief Overriden function, that sets a bounding polygon for a line with all its modifications.
     * @return path.
     */
    [[nodiscard]] QPainterPath shape() const override;

    /**
     * @brief Paints a with all additional attributes and modifications.
     * @param painter painters
     * @param option option
     * @param widget widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * @brief Creates a bounding box
     * @return QPolygonF bounding box
     */
    [[nodiscard]] QPolygonF lineShaper() const;

    /**
     * @brief Updates a _mergedActiveRegions list.
     */
    void updateActiveRegions();

    /**
     * @brief Creates intervals from asynchronous messages form a list
     * @param a A list ofo Pairs <ActoryType, SequenceConnectionItem>
     * @return List of merged regions
     */
    QList<region_t> getAsynchronousRegionsAsIntervals(ActorTypeConnectionList_t &a);

    /**
     * @brief Creates intervals for synchronous messages from a list
     * @details
     * Create pairs in such a way, that for a <= b <= `max`
     * they will look like<a, max> or <a, b>, if such b exists.
     * @param a A list ofo Pairs <ActoryType, SequenceConnectionItem>
     * @return List of merged regions
     */
    QList<region_t> getSynchronousRegionsAsIntervals(ActorTypeConnectionList_t &a);

    /**
     * @brief Returns the list with one number or an empty list if there are no creation points.
     * @return
     */
    QList<qreal> getFirstCreateRegion() const;

    /**
     * @brief Returns the list with one number or an empty list if there are no deletion points.
     * @return QList<qreal>
     */
    QList<qreal> getLastDeletePoint() const;

private:
    qreal _yFrom = 0; ///< starting point of the line (positition is a relative to the parent's position)
    qreal _height = 1000; ///< ending point of the line (positition is a relative to the parent's position)
    SequenceDiagramItem *_parent;

    ActorTypeConnectionList_t _activeRegions = ActorTypeConnectionList_t(); ///< regions for synchronous connections
    ActorTypeConnectionList_t _async_replyMessages = ActorTypeConnectionList_t(); ///< regions for all other connections

    QList<SequenceConnectionItem *> _createMessages = QList<SequenceConnectionItem *>(); ///< create messages. The minimum will be used
    QList<SequenceConnectionItem *> _deleteMessages = QList<SequenceConnectionItem *>(); ///< delete message. The maximum will be used
    QList<region_t> _mergedActiveRegions; ///< merged regions whech will marked as active

private:
    qreal const _actRegLen = 20.0; ///< lenght of an active region
    qreal _adjust = 10.0; ///< horisontal adjust for an active region
};

#endif //DIAGRAMS_SEQUENCEDIAGRAMLIFELINE_H

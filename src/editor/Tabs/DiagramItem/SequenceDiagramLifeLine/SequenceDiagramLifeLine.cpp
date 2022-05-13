/** @file SequenceDiagramLifeLine.cpp
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 04.05.2022
 */


#include "SequenceDiagramLifeLine.h"

#include <QPen>
#include <QPainter>

#define DEBUG 0

using namespace Connections;


SequenceDiagramLifeLine::SequenceDiagramLifeLine(SequenceDiagramItem *parent,
                                                 qreal yFrom,
                                                 qreal height)
        : QGraphicsLineItem(dynamic_cast<QGraphicsItem *>(parent)) {
    _height = height;
    _yFrom = yFrom;
    _parent = parent;

    // create a default line
    setZValue(-1.0);
    trackNodes();
}

SequenceDiagramLifeLine::~SequenceDiagramLifeLine() {}

QPolygonF SequenceDiagramLifeLine::lineShaper() const {
    QRectF rect(_parent->width() / 2 - _adjust, _parent->height(), 2 * _adjust, _height - _parent->pos().y());
    return QPolygonF(rect);
}

QPainterPath SequenceDiagramLifeLine::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}


void SequenceDiagramLifeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(255, 0, 0, 100), 0.5, Qt::DotLine));
    painter->drawPolygon(lineShaper());
#endif
    Q_UNUSED(option);
    Q_UNUSED(widget);

    auto clr = _parent->color();
    clr.setAlpha(_parent->color().alpha() / 2);
    painter->setBrush(QBrush(clr));
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));

    QList<QLineF> lines;
    auto topPoint = _parent->localCentre() + QPointF(0, _yFrom);
    updateActiveRegions();

    for (auto rect: _mergedActiveRegions) {
        // adjust a rectangle and draw it
        QPointF fromPoint{_parent->localCentre() + QPointF(-_adjust, rect.first - 50)};
        QPointF toPoint{_parent->localCentre() + QPointF(_adjust, rect.second - 50)};
        painter->drawRect(QRectF(fromPoint, toPoint));

        // add a line connecting two rectangles
        lines.push_back(QLineF(topPoint, fromPoint + QPointF(_adjust, 0)));
        topPoint = toPoint + QPointF(-_adjust, 0);
    }
    // prepare for drawing a line
    painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
    // add the last point.
    lines.push_front(QLineF(topPoint, _parent->localCentre() + QPointF(0.0, _height)));

    auto createConstraintList = getFirstCreateRegion();
    bool deleteCannotBeUsed = createConstraintList.isEmpty();
    auto startPosition = deleteCannotBeUsed ? 0 : createConstraintList.first();
    auto deleteConstraintList = getLastDeletePoint();
    auto deletePosition = deleteConstraintList.isEmpty() ? _height : deleteConstraintList.first();
    for (auto cLine: lines) {
        if (cLine.p1().y() < startPosition) {
            cLine = QLineF(QPointF(cLine.p1().x(), startPosition), cLine.p2());
        }
        if (cLine.p2().y() > deletePosition) {
            cLine = QLineF(cLine.p1(), QPointF(cLine.p2().x(), deletePosition));
        }
        if (cLine.p2().y() < startPosition || cLine.p1().y() > deletePosition) {
            continue;
        }
        painter->drawLine(cLine);
    }
}

void SequenceDiagramLifeLine::trackNodes() {
    auto line = QLineF(
            _parent->localCentre() + QPointF(0, _yFrom),
            _parent->localCentre() + QPointF(0, _height)
    );
    updateActiveRegions();
    setLine(line);
}

QList<region_t> SequenceDiagramLifeLine::getAsynchronousRegionsAsIntervals(
        QList<actorConnectionPair_t> &a
) {
    if (a.isEmpty()) {
        return QList<region_t>();
    }
    std::sort(a.begin(), a.end(),
              [](actorConnectionPair_t &a, actorConnectionPair_t &b) {
                  return a.second->y() < b.second->y();
              }
    );
    auto pairs = QList<region_t>();
    for (auto el: a) {
        pairs.push_back(region_t(el.second->y() - _actRegLen / 2 * (el.first == Caller), el.second->y() + _actRegLen));
    }
    return pairs;
}

QList<region_t> SequenceDiagramLifeLine::getSynchronousRegionsAsIntervals(
        QList<actorConnectionPair_t> &a
) {
    if (a.isEmpty()) {
        return QList<region_t>();
    }
    // sort an array in ascending order.
    std::sort(a.begin(), a.end(),
              [](actorConnectionPair_t &a, const actorConnectionPair_t &b) {
                  return a.second->y() < b.second->y();
              }
    );
    auto pairs = QList<region_t>();
    for (int i = 0; i < a.size(); i++) {
        if (i % 2 == 0) {
            // add an adjustment if type is caller.
            pairs.push_back(region_t(a[i].second->y() - _actRegLen / 2 * (a[i].first == Caller), _height));
        } else {
            auto tmp = pairs.takeLast();
            tmp.second = a[i].second->y();
            pairs.push_back(tmp);
        }
    }
    return pairs;
}

QList<qreal> SequenceDiagramLifeLine::getFirstCreateRegion() const {
    if (_createMessages.isEmpty()) {
        return {};
    }
    qreal minitem = _createMessages.first()->y();
    for (auto el: _createMessages) {
        if (el->y() < minitem) {
            minitem = el->y();
        }
    }
    return {minitem};
}

QList<qreal> SequenceDiagramLifeLine::getLastDeletePoint() const {
    if (_deleteMessages.isEmpty()) {
        return {};
    }
    qreal maxitem = _deleteMessages.first()->y();
    for (auto el: _deleteMessages) {
        if (el->y() > maxitem) {
            maxitem = el->y();
        }
    }
    return {maxitem};
}

void SequenceDiagramLifeLine::updateActiveRegions() {
    auto sf = [](const region_t &a, const region_t &b) {
        if (a.first < b.first) {return true;}
        if (a.first > b.first) {return false;}
        if (a.second < b.second) {return true;}
        return false;
    };

    _mergedActiveRegions.clear();
    _mergedActiveRegions.append(getSynchronousRegionsAsIntervals(_activeRegions));
    _mergedActiveRegions.append(getAsynchronousRegionsAsIntervals(_async_replyMessages));
    if (_mergedActiveRegions.isEmpty()) {
        return;
    }
    std::sort(_mergedActiveRegions.begin(), _mergedActiveRegions.end(), sf);

    // if object is not cretaed, yet, dont activate the connection.
    auto createConstraintList = getFirstCreateRegion();
    bool deleteCannotBeUsed = createConstraintList.isEmpty();
    auto startPosition = deleteCannotBeUsed ? 0 : createConstraintList.first();
    auto deleteConstraintList = getLastDeletePoint();
    auto deletePosition = deleteConstraintList.isEmpty() ? _height : deleteConstraintList.first();

    // remove overlapped intervals & check if they can be added
    for (int i = 0; i < _mergedActiveRegions.size() - 1;) {
        auto overlappedTop = _mergedActiveRegions[i].second >= _mergedActiveRegions[i + 1].first;
        auto overlappedBottom = _mergedActiveRegions[i].second >= _mergedActiveRegions[i + 1].second;
        if (overlappedTop || overlappedBottom) {
            // if two regions overlap, merge them
            _mergedActiveRegions[i].second = std::max(_mergedActiveRegions[i + 1].second,
                                                      _mergedActiveRegions[i].second);
            auto first = _mergedActiveRegions.size();
            _mergedActiveRegions.removeAt(i + 1);
            assert (_mergedActiveRegions.size() != first);
            // dont increase the counter
            continue;
        }
        i++;
    }
    int i = 0;
    while (true) {
        if (i == _mergedActiveRegions.size()) {
            break;
        }
        if (_mergedActiveRegions[i].first < startPosition || _mergedActiveRegions[i].first > deletePosition) {
            _mergedActiveRegions.removeAt(i);
        } else {
            if (_mergedActiveRegions[i].second > deletePosition) {
                _mergedActiveRegions[i].second = deletePosition;
            }
            i++;
        }
    }
}

void SequenceDiagramLifeLine::addConnection(
        SequenceConnectionItem *connection,
        ActorType actorType
) {
    switch (connection->connectionType()) {
        case Synchronous:
            if (!_activeRegions.contains(actorConnectionPair_t(actorType, connection))) {
                _activeRegions.push_back(actorConnectionPair_t(actorType, connection));
            }
            break;
        case Asynchronous:
            if (!_async_replyMessages.contains(actorConnectionPair_t(actorType, connection))) {
                _async_replyMessages.push_back(actorConnectionPair_t(actorType, connection));
            }
            break;
        case Reply:
            if (!_async_replyMessages.contains(actorConnectionPair_t(actorType, connection))) {
                _async_replyMessages.push_back(actorConnectionPair_t(actorType, connection));
            }
            break;
        case Create:
            if (actorType == Receiver) {
                if (!_createMessages.contains(connection)) {
                    _createMessages.push_back(connection);
                }
            } else { // Caller
                if (!_async_replyMessages.contains(actorConnectionPair_t(actorType, connection))) {
                    _async_replyMessages.push_back(actorConnectionPair_t(actorType, connection));
                }
            }
            break;
        case Delete:
            if (actorType == Receiver) {
                if (!_deleteMessages.contains(connection)) {
                    _deleteMessages.push_back(connection);
                }
            } else { // Caller
                if (!_async_replyMessages.contains(actorConnectionPair_t(actorType, connection))) {
                    _async_replyMessages.push_back(actorConnectionPair_t(actorType, connection));
                }
            }
            break;
        default:
            assert(false);
    }
}

void SequenceDiagramLifeLine::removeConnection(SequenceConnectionItem *connection) {
    auto callerC = actorConnectionPair_t(Caller, connection);
    auto receiverC = actorConnectionPair_t(Receiver, connection);
    auto list = {&_activeRegions, &_async_replyMessages};
    for (auto a: list) {
        if (a->contains(callerC)) {
            a->removeOne(callerC);
        } else if (a->contains(receiverC)) {
            a->removeOne(receiverC);
        }
    }
    if (_createMessages.contains(connection)) {
        _createMessages.removeOne(connection);
    }
    if (_deleteMessages.contains(connection)) {
        _deleteMessages.removeOne(connection);
    }
}
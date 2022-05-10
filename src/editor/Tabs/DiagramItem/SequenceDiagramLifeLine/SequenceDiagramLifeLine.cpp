// File: SequenceDiagramLifeLine.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022

#include "SequenceDiagramLifeLine.h"
#include "../DiagramItem.h"

#include <QPen>
#include <QPainter>
#include <QDebug>
#include "../../Connections/Connections.h"

#define DEBUG 1

using namespace Connections;

/** A constructor.
 *
 * @param yFrom the starting position of the line.
 * @param height the height of the line.
 */
SequenceDiagramLifeLine::SequenceDiagramLifeLine(SequenceDiagramItem *parent, qreal yFrom, qreal height)
        : QGraphicsLineItem(dynamic_cast<QGraphicsItem *>(parent)) {
    _height = height;
    _yFrom = yFrom;
    _parent = parent;
    _verticalAgjust = parent->pos().y();


    setFlags(QGraphicsItem::ItemIsSelectable);
    // create a default line
    trackNodes();
}

/** A destructor.
 *
 */
SequenceDiagramLifeLine::~SequenceDiagramLifeLine() {
}

/** Returns a bounding polygon for a line.
 *
 * @return bounding polygon.
 */
QPolygonF SequenceDiagramLifeLine::lineShaper() const {
    QRectF rect(_parent->width() / 2 - _adjust, _parent->height(), 2 * _adjust, _height);
    return QPolygonF(rect);
}

/** Overriden function, that sets a bounding polygon for a line with all its modifications.
 *
 * @return path.
 */
QPainterPath SequenceDiagramLifeLine::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}

/** Paints a with all additional attributes and modifications.
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceDiagramLifeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0, 100), 0.5, Qt::DotLine));
    painter->drawPolygon(lineShaper());
#endif
    qDebug() << __FILE__;
    auto clr = _parent->color();
    clr.setAlpha(_parent->color().alpha()/2);
    painter->setBrush(QBrush(clr));
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));

    QList<QLineF> lines;
    auto topPoint = _parent->localCentre() + QPointF(0, _yFrom);
    auto olapd = mergedActiveRegions();

    qDebug() << "     cretaed merged active regions";
    for (auto rect: olapd) {
        // adjust a rectangle and draw it
        QPointF fromPoint{_parent->localCentre() + QPointF(-_adjust, rect.first - 50)};
        QPointF toPoint{_parent->localCentre() + QPointF(_adjust, rect.second - 50)};
        painter->drawRect(QRectF(fromPoint, toPoint));

        // add a line connecting two rectangles
        lines.push_back(QLineF(topPoint, fromPoint + QPointF(_adjust, 0)));
        topPoint = toPoint + QPointF(-_adjust, 0);
    }
    qDebug() << "     fuck regions";
    // prepare for drawing a line
    painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
    // add the last point.
    lines.push_front(QLineF(topPoint, _parent->localCentre() + QPointF(0.0, _height + 0)));
    for (auto cLine: lines) {
        painter->drawLine(cLine);
    }
}

/** Reacts on a mouse press event, and selecting the line
 *
 * @param event
 */
void SequenceDiagramLifeLine::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        setSelected(true);
    }
}

/** Tracks nodes and updates the line
 *
 */
void SequenceDiagramLifeLine::trackNodes() {
    auto line = QLineF(
            _parent->localCentre() + QPointF(0, _yFrom),
            _parent->localCentre() + QPointF(0, _height)
    );
    mergedActiveRegions();
    setLine(line);
}

/**
 * Create pairs in such a way, that for a <= b <= `max`
 * they will look like<a, max> or <a, b>, if such b exists.
 *
 * @param a
 * @return
 */
QList<QPair<qreal, qreal>> SequenceDiagramLifeLine::getActiveRegionsAsIntervals(
        QList<const SequenceConnectionItem *> &a
) {
    // sort array
    qDebug() << __FILE__;

    if (a.isEmpty()) {
        return QList<QPair<qreal, qreal>>();
    }
    std::sort(a.begin(), a.end(),
              [](const SequenceConnectionItem *a, const SequenceConnectionItem *b) {
                  return a->y() < b->y();
              }
    );
    auto pairs = QList<QPair<qreal, qreal>>();
    for (uint32_t i = 0; i < a.size(); i++) {
        if (i % 2 == 0) {
            pairs.push_back(QPair<qreal, qreal>(a[i]->y(), _height));
        } else {
            pairs[i - 1].second = a[i]->y();
        }
    }
    qDebug() << "   there must be an array sorted in the ascending order";
    qDebug() << "   " << pairs;
    return pairs;
}

/** Merges all intervals
 *
 * @return merged intervals
 */
QList<QPair<qreal, qreal>> SequenceDiagramLifeLine::mergedActiveRegions() {
    qDebug() << __FILE__;
    qDebug() << "<";
    auto sf = [](const QPair<qreal, qreal> &a, const QPair<qreal, qreal> &b) {
        if (a.first < b.first) {return true;}
        if (a.first > b.first) {return false;}
        if (a.second < b.second) {return true;}
        return false;
    };
    auto a = QList<QPair<qreal, qreal>>();

    a.append(getActiveRegionsAsIntervals(_activeRegions));
//    a.append(_messagesRegionsNotSynchronous); // TODO
    if (a.isEmpty()) {
        return a;
    }
    std::sort(a.begin(), a.end(), sf);
    qDebug() << "    a sorted";
    // remove overlapped intervals
    for (int i = 0; i < a.size() - 1;) {
        if (a[i].second >= a[i + 1].second || a[i].second >= a[i + 1].first) {
            a[i].second = std::max(a[i + 1].second, a[i].second);
            a.remove(i + 1);
        } else {
            i++;
        }
    }
    qDebug() << "    overlapped chunks done";
    // adjust the size of a lifeline
//    updateHeight();
    qDebug() << ">";
    return a;
}

/**
 *
 * @param connection
 */
void SequenceDiagramLifeLine::addConnection(
        SequenceConnectionItem *connection,
        ActorType actorType
) {
    // TODO: improve margin for creation/deletion
//    auto margin = 15.0;
//    auto topMargin = actorType == Receiver ? 0 : margin;
//    auto bottomMargin = actorType == Receiver ? 0 : margin;

    qDebug() << "<";
    qDebug() << __FILE__;
    if (connection->connectionType() == Synchronous) {
        _activeRegions.push_back(connection);
    } else {
        assert(!"    TODO: make everything as an array of connections");
        _messagesRegionsNotSynchronous.push_back(QPair<qreal, qreal>(10, 10 + 10));
    }

    switch (actorType) {
        case Caller:
            qDebug() << "   caller";
            break;

        case Receiver:
            qDebug() << "   receiver";
            break;
    }
    qDebug() << ">";
}

/**
 *
 * @param connection
 */
void SequenceDiagramLifeLine::removeConnection(qreal y) {
    assert(!"remove connection coordinate from thelist");
}

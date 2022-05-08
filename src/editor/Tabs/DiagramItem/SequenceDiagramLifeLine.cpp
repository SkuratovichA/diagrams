// File: SequenceDiagramLifeLine.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022

#include "SequenceDiagramLifeLine.h"
#include "DiagramItem.h"

#include <QPen>
#include <QPainter>
#include <QDebug>

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
    setFlags(QGraphicsItem::ItemIsSelectable);
    // create a default line
    trackNodes();
}

/** A destructor.
 *
 */
SequenceDiagramLifeLine::~SequenceDiagramLifeLine() {}

/** Returns a bounding polygon for a line.
 *
 * @return bounding polygon.
 */
QPolygonF SequenceDiagramLifeLine::lineShaper() const {
    QRectF rect(_parent->width() / 2 - 10, _parent->height(), 20, maxHeight());
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
#if 1
    painter->setPen(QPen(QColor(0, 0, 0, 100), 0.5, Qt::DotLine));
    painter->drawPolygon(lineShaper());
#endif
    painter->setRenderHint(QPainter::Antialiasing, true);

    auto topPoint = QPointF(_parent->width()/2, _yFrom + _parent->height());
    QList<QLineF> lines;

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter->setBrush(QBrush(_parent->color()));
    auto olapd = mergedActiveRegions();
    auto const adjust = 10.0;
    for (auto rect: olapd) {
        QPointF fromPoint{-adjust + _parent->width() / 2, rect.first + _parent->height()};
        QPointF toPoint = QPointF(adjust + _parent->width() / 2,
                                  _parent->height() + (rect.second == -1 ? rect.first + _actRegLen : rect.second));
        painter->drawRect(QRectF(fromPoint, toPoint));

        lines.push_back(QLineF(topPoint, fromPoint + QPointF(adjust, 0)));
        topPoint = toPoint + QPointF(-adjust, 0);
    }

    painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
    lines.push_front(QLineF(topPoint, QPointF(_parent->width()/2, _height)));
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
            _parent->centre() + QPointF(0, _yFrom),
            _parent->centre() + QPointF(0, maxHeight())
    );
    setLine(line);
}

/** Merges all intervals
 *
 * @return merged intervals
 */
QList<QPair<qreal, qreal>> SequenceDiagramLifeLine::mergedActiveRegions() {
    auto sf = [](const QPair<qreal, qreal> &a, const QPair<qreal, qreal> &b) {
        if (a.first < b.first) {
            return true;
        }
        if (a.first > b.first) {
            return false;
        }
        if (a.second < b.second) {
            return true;
        }
    };
    QList<QPair<qreal, qreal>> a(_activeRegions);
    std::sort(a.begin(), a.end(), sf);
    // remove overlapped intervals
    for (int i = 0; i < a.size() - 1;) {
        if (a[i].second >= a[i + 1].second || a[i].second >= a[i + 1].first) {
            a[i].second = std::max(a[i + 1].second, a[i].second);
            a.remove(i + 1);
        } else {
            i++;
        }
    }
    return a;
}

/** Determines the maximum possible height of the life line.
 *
 * @return maximum height for the line line.
 */
qreal SequenceDiagramLifeLine::maxHeight() const {
    if (_activeRegions.isEmpty()) {
        return _height;
    }
    return std::max(_activeRegions.last().second, _height);
}

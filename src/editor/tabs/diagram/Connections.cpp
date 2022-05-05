// File: Connections.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#define DEBUG 1

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QStyle>
#include <QPainter>

#include "Connections.h"
#include "DiagramItem.h"

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
ClassConnectionItem::ClassConnectionItem(ClassDiagramItem *fromNode,
                                         ClassDiagramItem *toNode,
                                         ClassConnectionType connectionType,
                                         uint32_t order,
                                         QColor color
) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    _nodeFrom = fromNode;
    _nodeTo = toNode;

    _nodeFrom->addConnection(this);
    _nodeTo->addConnection(this);
    _connectionType = connectionType;
    _color = color;
    _order = order;

    setZValue(-1.0);
    trackNodes();
}

/**
 *
 * @param x1y1
 * @param x2y2
 * @param collision
 * @return
 */
inline int getOctant(QPointF const &x1y1, QPointF const &x2y2, bool *collision = nullptr) {
    auto x2 = x2y2.x() - x1y1.x(); // x2
    auto y2 = x2y2.y() - x1y1.y(); // y2
    qreal x1 = 0.0;
    qreal y1 = 0.0;

    x1 = y1 = 0; // x1
    if (x2 < x1) { // 3..6
        if (y2 < y1) { // 3,4
            if (collision != nullptr && x2 == y2) {
                *collision = true;
            }
            return x2 > y2 ? 3 : 4;
        } else {
            if (collision != nullptr && -x2 == y2) {
                *collision = true;
            }
            return -x2 > y2 ? 5 : 6;
        }
    } else { // 2..8
        if (y2 < y1) { // 1,2
            if (collision != nullptr && -x2 == y2) {
                *collision = true;
            }
            return -x2 >= y2 ? 2 : 1;
        } else { // 7,8
            if (collision != nullptr && x2 == y2) {
                *collision = true;
            }
            return x2 < y2 ? 7 : 8;
        }
    }
}

/**
 *
 * @return
 */
QPair<QPointF, QPointF> ClassConnectionItem::edgePoints() const {
    auto xFrom = _nodeFrom->centre().x(); // this will be changed to the custom point? FIXME
    auto yFrom = _nodeFrom->centre().y();// this will be changed to the custom point? FIXME
    auto xTo = _nodeTo->centre().x();// this will be changed to the custom point? FIXME
    auto yTo = _nodeTo->centre().y();// this will be changed to the custom point? FIXME

    auto octant = getOctant(QPointF(xFrom, yFrom), QPointF(xTo, yTo));
    auto a = yTo - yFrom;
    auto b = xFrom - xTo;
    auto c = -(a * xFrom + b * yFrom);

    switch (octant) {
        case 1:
        case 8:
            xTo = xTo - _nodeTo->width() / 2; // get the x-axis of the edge point
            yTo = -(c + a * xTo) / b; // using analytical geometry, compute the coords

            xFrom = xFrom + _nodeFrom->width() / 2;// get the x-axis of the edge point
            yFrom = -(c + a * xFrom) / b;
            break;
        case 2:
        case 3:
            yTo = yTo + _nodeTo->height() / 2;// get the x-axis of the edge point
            xTo = -(c + b * yTo) / a;

            yFrom = yFrom - _nodeFrom->height() / 2;// get the x-axis of the edge point
            xFrom = -(c + b * yFrom) / a;
            break;
        case 4:
        case 5:
            xTo = xTo + _nodeTo->width() / 2;// get the x-axis of the edge point
            yTo = -(c + a * xTo) / b;

            xFrom = xFrom - _nodeFrom->width() / 2;// get the x-axis of the edge point
            yFrom = -(c + a * xFrom) / b;
            break;
        case 6:
        case 7:
            yTo = yTo - _nodeTo->height() / 2;// get the x-axis of the edge point
            xTo = -(c + b * yTo) / a;

            yFrom = yFrom + _nodeFrom->height() / 2;// get the x-axis of the edge point
            xFrom = -(c + b * yFrom) / a;
            break;
    }
    return {QPointF(xFrom, yFrom), QPointF(xTo, yTo)};
}

/**
 *
 */
void ClassConnectionItem::trackNodes() {
    auto edgePs = edgePoints();
    setLine(QLineF(edgePs.first, edgePs.second));
}

/**
 *
 * @return
 */
QPolygonF ClassConnectionItem::lineShaper() const {
    QPolygonF poly;

    auto const fromUpperLeft = _nodeFrom->pos();
    auto const fromUpperRight = fromUpperLeft + QPointF(_nodeFrom->width(), 0);
    auto const fromLowerLeft = fromUpperLeft + QPointF(0, _nodeFrom->height());
    auto const fromLowerRight = fromLowerLeft + QPointF(_nodeFrom->width(), 0);

    auto const toUpperLeft = _nodeTo->pos();
    auto const toUpperRight = toUpperLeft + QPointF(_nodeTo->width(), 0);
    auto const toLowerLeft = toUpperLeft + QPointF(0, _nodeTo->height());
    auto const toLowerRight = toUpperRight + QPointF(0, _nodeTo->height());

#define ovlp(point) \
        (fromUpperLeft.x() < (point).x() && fromUpperLeft.y() < (point).y() \
        && fromUpperRight.x() > (point).x() && fromUpperRight.y() < (point).y() \
        && fromLowerLeft.y() > (point).y())

    auto ovlpEdges = ovlp(toUpperLeft) + ovlp(toUpperRight) + ovlp(toLowerLeft) + ovlp(toLowerRight);

    auto boundSize = 17.0;

    QPointF bLeft(std::min<qreal>(toUpperLeft.x(), fromUpperLeft.x()) - boundSize,
                  std::min<qreal>(toUpperLeft.y(), fromUpperLeft.y()) - boundSize);
    QPointF bRight(std::max<qreal>(toLowerRight.x(), fromLowerRight.x()) + boundSize,
                   std::max<qreal>(toLowerRight.y(), fromLowerRight.y()) + boundSize);

    QPointF topAdjuster;
    QPointF bottomAdjuster;
    bool collision = false;
    // FIXME: change to the custom centre of the point
    auto const octant = getOctant(_nodeFrom->centre(), _nodeTo->centre(), &collision);
    switch (ovlpEdges) {
        case 4:
        case 3:
        case 2:
        case 1: // only one overlapped edge - do not print bb
            poly << QPolygonF(QRectF(bLeft, bRight)); // bounding rectangle
            break;
        default:
            auto ePs = edgePoints();
            auto const farLeft = ePs.first;
            auto const farRight = ePs.second;

            switch (octant) {
                case 1:
                case 8:
                    if (collision) {
                        bottomAdjuster = QPointF(0, -boundSize);
                        topAdjuster = QPointF(-boundSize, 0);
                    } else {
                        topAdjuster = QPointF(-boundSize, boundSize);
                        bottomAdjuster = QPointF(-boundSize, -boundSize);
                    }
                    break;
                case 4:
                case 5:
                    if (collision) {
                        topAdjuster = QPointF(0, +boundSize);
                        bottomAdjuster = QPointF(+boundSize, +0);
                    } else {
                        topAdjuster = QPointF(boundSize, boundSize);
                        bottomAdjuster = QPointF(boundSize, -boundSize);
                    }
                    break;
                case 2:
                case 3:
                    if (collision) {
                        topAdjuster = QPointF(-boundSize, 0);
                        bottomAdjuster = QPointF(0, boundSize);
                    } else {
                        topAdjuster = QPointF(-boundSize, boundSize);
                        bottomAdjuster = QPointF(boundSize, boundSize);
                    }
                    break;
                case 6:
                case 7:
                    if (collision) {
                        topAdjuster = QPointF(0, -boundSize);
                        bottomAdjuster = QPointF(0, boundSize);
                    } else {
                        topAdjuster = QPointF(-boundSize, -boundSize);
                        bottomAdjuster = QPointF(boundSize, -boundSize);
                    }
                    break;
            }

            poly << farLeft + topAdjuster << farLeft + bottomAdjuster << farRight - topAdjuster
                 << farRight - bottomAdjuster;
            // **************
    }
    return poly;
#undef ovlp
}

/**
 *
 * @return
 */
QPainterPath ClassConnectionItem::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}

/**
 *
 */
void ClassConnectionItem::drawLine(QPainter *painter, const QStyleOptionGraphicsItem *option) const {
    painter->setPen(QPen(_color, 2, Qt::SolidLine));
    painter->setRenderHint(QPainter::Antialiasing, true);
    auto points = edgePoints();
    QPolygonF poly;
    QLineF cLine = line();
    qreal angle = std::atan2(-cLine.dy(), cLine.dx());
    auto scale = 20;
    poly.clear();

    if (option->state & QStyle::State_Selected) {
        painter->setPen(QPen(_color.darker(), 2.5, Qt::SolidLine));
    }
    QPointF arrowP1 = cLine.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * scale,
                                           cos(angle + M_PI - M_PI / 3) * scale);
    QPointF arrowP2 = cLine.p1() + QPointF(sin(angle + M_PI / 3) * scale,
                                           cos(angle + M_PI / 3) * scale);
    QPointF arrowP3 = cLine.p1() + QPointF(sin(angle + M_PI / 2) * scale * 1.7,
                                           cos(angle + M_PI / 2) * scale * 1.7);
    QColor clr(_color);
    switch (_connectionType) {
        case Association:
            break;
        case Aggregation:
            clr = QColor(Qt::white);
        case Composition:
            poly << cLine.p1() << arrowP1 << arrowP3 << arrowP2;
            break;
        case Dependency:
            painter->setPen(QPen(_color, 2, Qt::DashLine));
            clr = QColor(Qt::white);
        case Generalization:
            poly << cLine.p1() << arrowP1 << arrowP2;
            break;
    }
    painter->setBrush(clr);
    painter->drawLine(cLine);
    painter->drawPolygon(poly);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void ClassConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(0,0,0,100), 0.5, Qt::DotLine));
    painter->drawPolygon(lineShaper());
#endif
    drawLine(painter, option);
}

/**
 *
 */
ClassConnectionItem::~ClassConnectionItem() {
    qDebug() << "removing connections from 'fromNode' (Connections.cpp)";
    if (_nodeFrom != nullptr) {
        _nodeFrom->removeConnection(this);
    }
    qDebug() << "\tremoved";

    qDebug() << "removing connections from 'toNode' (Conneictions.cpp)";
    if (_nodeTo != nullptr) {
        _nodeTo->removeConnection(this);
    }
    qDebug() << "\tremoved";
}

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
ActorConnectionItem::ActorConnectionItem(ActorDiagramItem *fromNode,
                                         ActorDiagramItem *toNode,
                                         ActorConnectionType connectionType) {
    nodeFrom = fromNode;
    nodeTo = toNode;

    nodeFrom->addConnection(this);
    nodeTo->addConnection(this);

    setZValue(-1.0);
//    setColor(Qt::darkRed);
    trackNodes();
}

/**
 *
 */
ActorConnectionItem::~ActorConnectionItem() {
    if (nodeFrom != nullptr) {
        nodeFrom->removeConnection(this);
    }
    if (nodeTo != nullptr) {
        nodeTo->removeConnection(this);
    }
}
//
///**
// *
// * @param color
// */
//void ActorConnectionItem::setColor(const QColor &color) {
//    setPen(QPen(color, 1.0));
//}

/**
 *
 * @return
 */
QColor ActorConnectionItem::color() const {
    return pen().color();
}

/**
 *
 */
void ActorConnectionItem::trackNodes() {
    setLine(QLineF(nodeFrom->pos(), nodeTo->pos()));
}

/**
 *
 * @return
 */
ActorDiagramItem *ActorConnectionItem::fromNode() const {
    return nodeFrom;
}

/**
 *
 * @return
 */
ActorDiagramItem *ActorConnectionItem::toNode() const {
    return nodeTo;
}

void ClassConnectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        setSelected(true);
    }
}

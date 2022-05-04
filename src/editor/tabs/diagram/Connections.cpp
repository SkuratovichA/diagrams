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
                                         ClassConnectionType connectionType) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    nodeFrom = fromNode;
    nodeTo = toNode;

    nodeFrom->addConnection(this);
    nodeTo->addConnection(this);

    setZValue(-1.0);

    setColor(Qt::darkRed);
    trackNodes();
}

/**
 *
 */
ClassConnectionItem::~ClassConnectionItem() {
    qDebug() << "removing connections from 'fromNode'";
    if (nodeFrom != nullptr) {
        nodeFrom->removeConnection(this);
    }
    qDebug() << "DONE";

    qDebug() << "removing connections from 'toNode'";
    if (nodeTo != nullptr) {
        nodeTo->removeConnection(this);
    }
    qDebug() << "DONE";
}

/**
 *
 * @return
 */
ClassDiagramItem *ClassConnectionItem::fromNode() const {
    return nodeFrom;
}

/**
 *
 * @return
 */
ClassDiagramItem *ClassConnectionItem::toNode() const {
    return nodeTo;
}

/**
 *
 * @param color
 */
void ClassConnectionItem::setColor(const QColor &color) {
    setPen(QPen(color, 1.0));
}

/**
 *
 * @return
 */
QColor ClassConnectionItem::color() const {
    return pen().color();
}

/**
 *
 */
void ClassConnectionItem::trackNodes() {
    setLine(QLineF(nodeFrom->centre(), nodeTo->centre()));
}

inline int getOctant(QPointF const &x1y1, QPointF const &x2y2) {
    auto x2 = x2y2.x() -x1y1.x(); // x2
    auto y2 = x2y2.y() -x1y1.y(); // y2
    qreal x1 = 0.0;
    qreal y1 = 0.0;

    x1 = y1 = 0; // x1
    if (x2 < x1) { // 3..6
        if (y2 < y1) { // 3,4
            return x2 > y2 ? 3 : 4;
        } else {
            return -x2 > y2 ? 5 : 6;
        }
    } else { // 2..8
        if (y2 < y1) { // 1,2
            return -x2 >= y2 ? 2 : 1;
        } else { // 7,8
            return x2 < y2 ? 7 : 8;
        }
    }
}

QPair<QPointF, QPointF> edgePoints(QPointF const x1y1, QPointF const x2y2, QPointF const  &whFrom, QPointF const &whTo) {
    auto fromW = whFrom.x();
    auto fromH = whFrom.y();
    auto toW = whTo.x();
    auto toH = whTo.y();

    auto octant = getOctant(x1y1, x2y2);
    auto a = x2y2.y() - x1y1.x();
    auto b = x1y1.x() - x2y2.x();
    auto c = -(a*x1y1.x() + b*x1y1.y()); // dot product TODO:

    qreal xTo = x2y2.x();
    qreal yTo = x2y2.y();
    qreal xFrom = x1y1.x();
    qreal yFrom = x1y1.y();
    switch (octant) {
        case 1: case 8:
            xTo = xTo - toW / 2;
            yTo = -(c + a*xTo) / b;

            xFrom = xFrom + fromW / 2;
            yFrom = -(c + a*xFrom) / b;
            break;
        case 2: case 3:
            yTo = yTo + toH / 2;
            xTo = -(c + b*yTo) / a;

            yFrom = yFrom - fromH / 2;
            xFrom = -(c + b*yFrom) / a;
            break;
        case 4: case 5:
            xTo = xTo + toW / 2;
            yTo = -(c + a*xTo) / b;

            xFrom = xFrom - fromW / 2;
            yFrom = -(c + a*xFrom) / b;
            break;
        case 6: case 7:
            yTo = yTo - toH / 2;
            xTo = -(c + b*yTo) / a;

            yFrom = yFrom + fromH / 2;
            xFrom = -(c + b*yFrom) / a;
            break;
        default: break;
    }

    return {QPointF(xFrom, yFrom), QPointF(xTo, yTo)};
}
//
//QPolygonF lineShaperr(QPointF const &fromPos, QPointF const &toPos, QPointF const &x1y1, QPointF &x2y2, QPointF) {
//    auto fromUpperLeft = pos;
//    auto fromUpperRight = pos + QPointF(fromWH.x(), 0);
//    auto fromLowerLeft = pos + QPointF(0, fromWH.y());
//    auto fromLowerRight = pos + fromWH;
//
//    auto toUpperLeft = pos;
//    auto toUpperRight = pos + QPointF(toWH.x(), 0);
//    auto toLowerLeft = pos + QPointF(0, toWH.y());
//    auto toLowerRight = pos + toWH;
//
//#define ovlp(point) \
//        (fromUpperLeft.x() < (point).x() && fromUpperLeft.y() < (point).y() \
//        && fromUpperRight.x() > (point).x() && fromUpperRight.y() < (point).y() \
//        && fromLowerLeft.y() > (point).y())
//
//    auto ovlpEdges = ovlp(toUpperLeft) + ovlp(toUpperRight) + ovlp(toLowerLeft) + ovlp(toLowerRight);
//
//    QPointF bLeft(std::min<qreal>(toUpperLeft.x(), fromUpperLeft.x()) - 15.0,
//                  std::min<qreal>(toUpperLeft.y(), fromUpperLeft.y()) - 15.0);
//    QPointF bRight(std::max<qreal>(toLowerRight.x(), fromLowerRight.x()) + 15.0,
//                   std::max<qreal>(toLowerRight.y(), fromLowerRight.y()) + 15.0);
//
//    auto octant = getOctant(x1y1, x2y2);
//
//    QPolygonF poly;
//    switch(ovlpEdges) {
//        case 4:
//        case 3:
//        case 2:
//        case 1: // only one overlapped edge - do not print bb
//            poly = QRectF(bLeft, bRight);
//            break;
//        default:
//            QPointF pa1; // adjustment 1
//            QPointF pa2; // adjustment 2
//            auto edges = edgePoints(x1y1, x2y2, fromWH, toWH);
//
//            // ************* dont use this fix
//            auto adjust = QPointF(+10.0, +10.0);
//            switch (octant) {
//                case 7: case 8: case 3: case 4:
//                    adjust.setX(-10.0);
//                default: break;
//            }
//            auto farLeft = nodeFrom->centre();
//            auto farRight = nodeTo->centre();
//            poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;
//            // **************
//    }
//}

QPainterPath ClassConnectionItem::shape() const {
    QPolygonF poly;
    QPainterPath path;

    auto x1y1 = nodeFrom->centre();
    auto x2y2 = nodeTo->centre();
    auto diff = (x2y2 - x1y1);
    auto fromWH = QPointF(nodeFrom->width(),nodeFrom->height());
    auto toWH = QPointF(nodeTo->width(), nodeTo->height());
    auto octant = getOctant(x1y1, x2y2);

    auto fromUpperLeft = nodeFrom->pos();
    auto fromUpperRight = fromUpperLeft + QPointF(nodeFrom->width(), 0);
    auto fromLowerLeft = fromUpperLeft + QPointF(0, nodeFrom->height());
    auto fromLowerRight = fromLowerLeft + QPointF(nodeFrom->width(), 0);

    auto toUpperLeft = nodeTo->pos();
    auto toUpperRight = toUpperLeft + QPointF(nodeTo->width(), 0);
    auto toLowerLeft = toUpperLeft + QPointF(0, nodeTo->height());
    auto toLowerRight = toUpperRight + QPointF(0, nodeTo->height());

    #define ovlp(point) \
        (fromUpperLeft.x() < (point).x() && fromUpperLeft.y() < (point).y() \
        && fromUpperRight.x() > (point).x() && fromUpperRight.y() < (point).y() \
        && fromLowerLeft.y() > (point).y())

    auto ovlpEdges = ovlp(toUpperLeft) + ovlp(toUpperRight) + ovlp(toLowerLeft) + ovlp(toLowerRight);

    QPointF bLeft(std::min<qreal>(toUpperLeft.x(), fromUpperLeft.x()) - 15.0,
                  std::min<qreal>(toUpperLeft.y(), fromUpperLeft.y()) - 15.0);
    QPointF bRight(std::max<qreal>(toLowerRight.x(), fromLowerRight.x()) + 15.0,
                   std::max<qreal>(toLowerRight.y(), fromLowerRight.y()) + 15.0);

    switch(ovlpEdges) {
        case 4:
        case 3:
        case 2:
        case 1: // only one overlapped edge - do not print bb
            path.addRect(QRectF(bLeft, bRight)); // bounding rectangle
            break;
        default:
            // ************* dont use this fix
            auto adjust = QPointF(+10.0, +10.0);
            switch (octant) {
                case 7: case 8: case 3: case 4:
                    adjust.setX(-10.0);
                default: break;
            }
            auto farLeft = nodeFrom->centre();
            auto farRight = nodeTo->centre();

            QPolygonF poly; // bounding polygon
            poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;

            path.addPolygon(poly);
            // **************
    }
    return path;
#undef ovlp
}

void ClassConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    auto x1y1 = nodeFrom->centre();
    auto x2y2 = nodeTo->centre();
    auto fromWH = QPointF(nodeFrom->width(), nodeFrom->height());
    auto toWH = QPointF(nodeTo->width(), nodeTo->height());

#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0), 1, Qt::DotLine));

#if 1
    auto fromUpperLeft = nodeFrom->pos();
    auto fromUpperRight = nodeFrom->pos() + QPointF(nodeFrom->width(), 0);
    auto fromLowerLeft = nodeFrom->pos() + QPointF(0, fromWH.y());
    auto fromLowerRight = nodeFrom->pos() + fromWH;

    auto toUpperLeft = nodeTo->pos();
    auto toUpperRight = nodeTo->pos() + QPointF(toWH.x(), 0);
    auto toLowerLeft = nodeTo->pos() + QPointF(0, toWH.y());
    auto toLowerRight = nodeTo->pos() + toWH;

    #define ovlp(point) \
        (fromUpperLeft.x() < (point).x() && fromUpperLeft.y() < (point).y() \
        && fromUpperRight.x() > (point).x() && fromUpperRight.y() < (point).y() \
        && fromLowerLeft.y() > (point).y())

    auto ovlpEdges = ovlp(toUpperLeft) + ovlp(toUpperRight) + ovlp(toLowerLeft) + ovlp(toLowerRight);

    QPointF bLeft(std::min<qreal>(toUpperLeft.x(), fromUpperLeft.x()) - 15.0,
            std::min<qreal>(toUpperLeft.y(), fromUpperLeft.y()) - 15.0);
    QPointF bRight(std::max<qreal>(toLowerRight.x(), fromLowerRight.x()) + 15.0,
            std::max<qreal>(toLowerRight.y(), fromLowerRight.y()) + 15.0);

    auto octant = getOctant(x1y1, x2y2);

    QPolygonF poly;
    switch(ovlpEdges) {
        case 4:
        case 3:
        case 2:
        case 1: // only one overlapped edge - do not print bb
            poly = QRectF(bLeft, bRight);
            break;
        default:
            QPointF pa1; // adjustment 1
            QPointF pa2; // adjustment 2
            auto edges = edgePoints(x1y1, x2y2, fromWH, toWH);

            // ************* dont use this fix
            auto adjust = QPointF(+10.0, +10.0);
            switch (octant) {
                case 7: case 8: case 3: case 4:
                    adjust.setX(-10.0);
                default: break;
            }
            auto farLeft = nodeFrom->centre();
            auto farRight = nodeTo->centre();
            poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;
            // **************
    }
#endif
    painter->drawPolygon(poly);
#endif


    // ************** painting "arrows"
    QPen linepen(Qt::black);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(15);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(linepen);

    auto body = edgePoints(x1y1, x2y2, fromWH, toWH);
    painter->drawPoint(body.first);
    linepen.setColor(Qt::red);
    painter->setPen(linepen);
    painter->drawPoint(body.second);

    QLineF cLine = line();
    painter->setPen(QPen(QColor(218, 120, 218), 2, Qt::SolidLine));
    qreal lineAngle = cLine.angle();

//    const qreal radius = 2.0;
//    QLineF head1 = cLine;
//    head1.setAngle(lineAngle+32);
//    head1.setLength(100);
//    qDebug() << "line coordinates set:";
//    qDebug() << "\t (" << head1.x1() << "," << head1.y1() << ") -> (" << head1.x2() << ","<< head1.y2() << ")";

//    head1.setP1(QPointF(head1.x1()+70, head1.y1()));
//    head1.setP2(QPointF(head1.x2()-70, head1.y2()));
//    qDebug() << "line coordinates changed:";
//    qDebug() << "\t (" << head1.x1() << "," << head1.y1() << ") -> (" << head1.x2() << ","<< head1.y2() << ")";



//    QLineF head2 = cLine;
//    head2.setAngle(lineAngle-32);
//    head2.setLength(12);
//
//    painter->drawLine(head1);
//    painter->drawLine(head2);
//    painter->setPen(QPen(Qt::darkMagenta, 2, Qt::SolidLine));
//
//    painter->drawLine(cLine);
//    painter->drawLine(head1);
//    painter->drawLine(head2);

    if (option->state & QStyle::State_Selected) {
        painter->setPen(QPen(QColor(228, 120, 228), 3, Qt::SolidLine));
    }
    painter->drawLine(cLine);
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

    setColor(Qt::darkRed);
    trackNodes();
}

/**
 *
 */
ActorConnectionItem::~ActorConnectionItem() {
    nodeFrom->removeConnection(this);
    nodeTo->removeConnection(this);
}

/**
 *
 * @param color
 */
void ActorConnectionItem::setColor(const QColor &color) {
    setPen(QPen(color, 1.0));
}

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

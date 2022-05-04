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

QPair<QPointF, QPointF> ClassConnectionItem::edgePoints() const {
    auto xFrom   = nodeFrom->centre().x();
    auto yFrom   = nodeFrom->centre().y();
    auto xTo = nodeTo->centre().x();
    auto yTo = nodeTo->centre().y();

    auto octant = getOctant(QPoint(xFrom, yFrom), QPoint(xTo, yTo));
    auto a = yTo - yFrom;
    auto b = xFrom - xTo;
    auto c = -(a*xFrom +b*yFrom);

    switch (octant) {
        case 1: case 8:
            xTo = xTo - nodeTo->width() / 2;
            yTo = -(c + a*xTo) / b;

            xFrom = xFrom + nodeFrom->width() / 2;
            yFrom = -(c + a*xFrom) / b;
            break;
        case 2: case 3:
            yTo = yTo + nodeTo->height() / 2;
            xTo = -(c + b*yTo) / a;

            yFrom = yFrom - nodeFrom->height() / 2;
            xFrom = -(c + b*yFrom) / a;
            break;
        case 4: case 5:
            xTo = xTo + nodeTo->width() / 2;
            yTo = -(c + a*xTo) / b;

            xFrom = xFrom - nodeFrom->width() / 2;
            yFrom = -(c + a*xFrom) / b;
            break;
        case 6: case 7:
            yTo = yTo - nodeTo->height() / 2;
            xTo = -(c + b*yTo) / a;

            yFrom = yFrom + nodeFrom->height() / 2;
            xFrom = -(c + b*yFrom) / a;
            break;
        default: break;
    }
    return {QPointF(xFrom, yFrom), QPointF(xTo, yTo)};
}

QPolygonF ClassConnectionItem::lineShaper() const {
    QPolygonF poly;

    auto const octant = getOctant(nodeFrom->centre(), nodeTo->centre());

    auto const fromUpperLeft = nodeFrom->pos();
    auto const fromUpperRight = fromUpperLeft + QPointF(nodeFrom->width(), 0);
    auto const fromLowerLeft = fromUpperLeft + QPointF(0, nodeFrom->height());
    auto const fromLowerRight = fromLowerLeft + QPointF(nodeFrom->width(), 0);

    auto const toUpperLeft = nodeTo->pos();
    auto const toUpperRight = toUpperLeft + QPointF(nodeTo->width(), 0);
    auto const toLowerLeft = toUpperLeft + QPointF(0, nodeTo->height());
    auto const toLowerRight = toUpperRight + QPointF(0, nodeTo->height());

    #define ovlp(point) \
        (fromUpperLeft.x() < (point).x() && fromUpperLeft.y() < (point).y() \
        && fromUpperRight.x() > (point).x() && fromUpperRight.y() < (point).y() \
        && fromLowerLeft.y() > (point).y())

    auto ovlpEdges = ovlp(toUpperLeft) + ovlp(toUpperRight) + ovlp(toLowerLeft) + ovlp(toLowerRight);

    QPointF bLeft(std::min<qreal>(toUpperLeft.x(), fromUpperLeft.x()) - 15.0,
                  std::min<qreal>(toUpperLeft.y(), fromUpperLeft.y()) - 15.0);
    QPointF bRight(std::max<qreal>(toLowerRight.x(), fromLowerRight.x()) + 15.0,
                   std::max<qreal>(toLowerRight.y(), fromLowerRight.y()) + 15.0);

    auto adjust = QPointF(+10.0, +10.0);
    switch(ovlpEdges) {
        case 4: case 3: case 2: case 1: // only one overlapped edge - do not print bb
            poly << QPolygonF(QRectF(bLeft, bRight)); // bounding rectangle
            break;
        default:
            // ************* dont use this fix
            switch (octant) {
                case 7: case 8: case 3: case 4:
                    adjust.setX(-10.0);
                default: break;
            }
            auto const farLeft = nodeFrom->centre();
            auto const farRight = nodeTo->centre();

            poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;
            // **************
    }
    return poly;
#undef ovlp
}

QPainterPath ClassConnectionItem::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}

void ClassConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    auto x1y1 = nodeFrom->centre();
    auto x2y2 = nodeTo->centre();
    auto fromWH = QPointF(nodeFrom->width(), nodeFrom->height());
    auto toWH = QPointF(nodeTo->width(), nodeTo->height());

#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0), 1, Qt::DotLine));
    painter->drawPolygon(lineShaper());
#endif


    // ************** painting "arrows"
    QPen linepen(Qt::black);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(15);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(linepen);

    auto body = edgePoints();
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

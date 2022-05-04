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

QPainterPath ClassConnectionItem::shape() const {
    QPolygonF poly;
    QPainterPath path;

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

    if (ovlp(toUpperLeft) || ovlp(toUpperRight) || ovlp(toLowerLeft) || ovlp(toLowerRight)) {
        qDebug() << "overlapped!";
        auto xLeft = std::min<qreal>(toUpperLeft.x(), fromUpperLeft.x()) - 15.0;
        auto yLeft = std::min<qreal>(toUpperLeft.y(), fromUpperLeft.y()) - 15.0;
        auto xRight = std::max<qreal>(toLowerRight.x(), fromLowerRight.x()) + 15.0;
        auto yRight = std::max<qreal>(toLowerRight.y(), fromLowerRight.y()) + 15.0;

        path.addRect(QRectF(xLeft, yLeft, xRight, yRight));
    } else {

        auto x1 = nodeFrom->centre().x();
        auto y1 = nodeFrom->centre().y();
        auto x2 = nodeTo->centre().x();
        auto y2 = nodeTo->centre().y();
        // ************* dont use this fix
        auto adjust = QPointF(+10.0, +10.0);
        if (x1 <= x2 && y1 <= y2 || x1 >= x2 && y1 >= y2) {
            adjust.setX(-10.0);
        }
        auto farLeft = nodeFrom->centre();
        auto farRight = nodeTo->centre();

        poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;
        // **************

        path.addPolygon(poly);
    }
    return path;
#undef ovlp
}

inline int getOctant(qreal x1, qreal y1, qreal x2, qreal y2) {
    x2 = x2 - x1;
    y2 = y2 - y1;
    x1 = y1 = 0;
    if (x2 < x1) { // 3..6
        if (y2 < y1) { // 3,4
            if (x2 >= y2) {
                return 3;
            } else {
                return 4;
            }
        } else {
            if (-x2 >= y2) {
                return 5;
            } else {
                return 6;
            }
        }
    } else { // 2..8
        if (y2 < y1) { // 1,2
            if (-x2 >= y2) {
                return 2;
            } else {
                return 1;
            }
        } else { // 7,8
            if (x2 <= y2) {
                return 7;
            } else {
                return 8;
            }
        }
    }
}

void ClassConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0), 1, Qt::DotLine));
    auto x1 = nodeFrom->centre().x();
    auto y1 = nodeFrom->centre().y();
    auto x2 = nodeTo->centre().x();
    auto y2 = nodeTo->centre().y();
    auto adjust = QPointF(+10.0, +10.0);

    auto octant = getOctant(x1, y1, x2, y2);
    switch (octant) {
        case 7: case 8: case 3: case 4:
            adjust.setX(-10.0);
        default: break;
    }
    auto farLeft = nodeFrom->centre();
    auto farRight = nodeTo->centre();

    QPolygonF poly;
    poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;
    painter->drawPolygon(poly);


    QPen linepen(Qt::black);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(15);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(linepen);

    auto a = y2 - y1;
    auto b = x1 - x2;
    auto c = -(a*x1 +b*y1);
    //
    qreal xTo = x2;
    qreal yTo = y2;
    qreal xFrom = x1;
    qreal yFrom = y1;
    switch (octant) {
        case 1: case 8:
            xTo = x2 - nodeTo->width() / 2;
            yTo = -(c + a*xTo) / b;

            xFrom = x1 + nodeFrom->width() / 2;
            yFrom = -(c + a*xFrom) / b;
            break;
        case 2: case 3:
            yTo = y2 + nodeTo->height() / 2;
            xTo = -(c + b*yTo) / a;

            yFrom = y1 - nodeFrom->height() / 2;
            xFrom = -(c + b*yFrom) / a;
            break;
        case 4: case 5:
            xTo = x2 + nodeTo->width() / 2;
            yTo = -(c + a*xTo) / b;

            xFrom = x1 - nodeFrom->width() / 2;
            yFrom = -(c + a*xFrom) / b;
            break;
        case 6: case 7:
            yTo = y2 - nodeTo->height() / 2;
            xTo = -(c + b*yTo) / a;

            yFrom = y1 + nodeFrom->height() / 2;
            xFrom = -(c + b*yFrom) / a;
           break;
       default: break;
    }
    painter->drawPoint(xFrom, yFrom);
    linepen.setColor(Qt::red);
    painter->setPen(linepen);
    painter->drawPoint(xTo, yTo);
#endif

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

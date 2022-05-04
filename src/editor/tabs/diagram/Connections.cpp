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
    qDebug() << "removing connections";
    nodeFrom->removeConnection(this);
    nodeTo->removeConnection(this);
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
    auto x1 = nodeFrom->centre().x();
    auto y1 = nodeFrom->centre().y();
    auto x2 = nodeTo->centre().x();
    auto y2 = nodeTo->centre().y();
    auto adjust = QPointF(+10.0, +10.0);

    if (x1 <= x2 && y1 <= y2 || x1 >= x2 && y1 >= y2) {
        adjust.setX(-10);
    }
    auto farLeft = nodeFrom->centre();
    auto farRight = nodeTo->centre();

    QPolygonF poly;
    poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;
    QPainterPath path;
    path.addPolygon(poly);
    return path;
}

void ClassConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0), 1, Qt::DotLine));
    auto x1 = nodeFrom->centre().x();
    auto y1 = nodeFrom->centre().y();
    auto x2 = nodeTo->centre().x();
    auto y2 = nodeTo->centre().y();
    auto adjust = QPointF(+10.0, +10.0);

    if (x1 <= x2 && y1 <= y2 || x1 >= x2 && y1 >= y2) {
        adjust.setX(-10);
    }
    auto farLeft = nodeFrom->centre();
    auto farRight = nodeTo->centre();

    QPolygonF poly;
    poly << farLeft + adjust << farLeft - adjust << farRight - adjust << farRight + adjust;
    painter->drawPolygon(poly);
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
